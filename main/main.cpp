
#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture{
    public:

        LTexture();
        ~LTexture();

        bool LoadFromFile(std::string path);

        void free();

        void render(int x, int y, SDL_Rect* clip = NULL);

        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

bool init();
bool media();
void close();

SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;

SDL_Rect gSpriteSheet[4];

LTexture gSheet;

LTexture::LTexture(){

    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;

}

LTexture::~LTexture(){

    free();

}

void LTexture::free(){

    if (mTexture != NULL){

        SDL_DestroyTexture(mTexture);
        mWidth = 0;
        mHeight = 0;

    }

}

void LTexture::render(int x, int y, SDL_Rect* clip){

    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if (clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(Renderer, mTexture, clip , &renderQuad);

}

 LTexture::getWidth(){
    return mWidth;
}

 LTexture::getHeight(){
    return mHeight;
}

bool init(){

    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Could not initialize SDL! SDL error: " << std::endl;
        SDL_GetError();
        success = false;
    }
    else{
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
        Window = SDL_CreateWindow("Adventure time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Window == NULL){
            std::cout << "Could not create window! SDL error: " << std::endl;
            SDL_GetError();
            success = false;
        }
        else{
            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
            if (Renderer == NULL){
                std::cout << "Could not create renderer! SDL error: " << std::endl;
                SDL_GetError();
                success = false;
            }
            else{
                SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    std::cout << "Could not initialize image flags! IMG error: " <<  std::endl;
                    IMG_GetError();
                    success = false;
                }
            }

        }
    }
    return success;
}

bool LTexture::LoadFromFile(std::string path){

    SDL_Texture* newTexture = NULL;

    SDL_Surface* Image_Surface = IMG_Load(path.c_str());

    if (Image_Surface == NULL){
        std::cout << "Could not load image surface! IMG error: " ;
        IMG_GetError();
    }
    else{
        SDL_SetColorKey(Image_Surface, SDL_TRUE, SDL_MapRGB(Image_Surface->format, 0, 255, 255));

        newTexture = SDL_CreateTextureFromSurface(Renderer, Image_Surface);
        if (newTexture == NULL){
            std::cout << "Unable to create texture from surface! SDL2 error: ";
            SDL_GetError();
        }
        else{
            mWidth = Image_Surface->w;
            mHeight = Image_Surface->h;
        }
    }

    SDL_FreeSurface(Image_Surface);
    mTexture = newTexture;

    return mTexture != NULL;
}

bool media(){

    bool success = true;

    if (!gSheet.LoadFromFile("main/dots.png")){
        std::cout << "Could not load sheet image! SDL error: " ;
        SDL_GetError();
        success = false;
    }
    else{
        gSpriteSheet[0].x = 0;
        gSpriteSheet[0].y = 0;
        gSpriteSheet[0].w = 100;
        gSpriteSheet[0].h = 100;

        gSpriteSheet[1].x = 100;
        gSpriteSheet[1].y = 0;
        gSpriteSheet[1].w = 100;
        gSpriteSheet[1].h = 100;

        gSpriteSheet[2].x = 0;
        gSpriteSheet[2].y = 100;
        gSpriteSheet[2].w = 100;
        gSpriteSheet[2].h = 100;

        gSpriteSheet[3].x = 100;
        gSpriteSheet[3].y = 100;
        gSpriteSheet[3].w = 100;
        gSpriteSheet[3].h = 100;
    }

    return success;
}

void close(){

    gSheet.free();

    SDL_DestroyRenderer(Renderer);
    Renderer = NULL;
    SDL_DestroyWindow(Window);
    Window = NULL;

    IMG_Quit();
    SDL_Quit();

}

int main(int argc, char* args[]){

    if (!init()){
        std::cout << "Could not initialize sdl2! sdl error: " << std::endl;
        SDL_GetError();
    }
    else{
        if (!media()){
            std::cout << "Could not load image surface! img error: " << std::endl;
            IMG_GetError();
        }
        else{
            bool quit = false;

            SDL_Event e;

            while (!quit){
                while (SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_QUIT){
                        quit = true;
                    }
                }

                //Clear screen
				SDL_SetRenderDrawColor( Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( Renderer );

				gSheet.render(0, 0, &gSpriteSheet[0]);

				gSheet.render(SCREEN_WIDTH - gSpriteSheet[1].w, 0, &gSpriteSheet[1]);

				gSheet.render(0, SCREEN_HEIGHT - gSpriteSheet[2].h, &gSpriteSheet[2]);

				gSheet.render(SCREEN_WIDTH - gSpriteSheet[3].w, SCREEN_HEIGHT - gSpriteSheet[3].h, &gSpriteSheet[3]);

				SDL_RenderPresent( Renderer );

            }
        }
    }

    void close();

    return 0;
}
