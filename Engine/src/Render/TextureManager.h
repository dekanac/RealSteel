

namespace Engine
{
    class Renderer;
    struct Texture;
    struct AnimationTexture;

    class TextureManager
    {
    public:
        bool CreateTexture(Renderer* renderer_, std::string name_, std::string path_);
        bool CreateTexture(Renderer* renderer_, std::string name_, std::string path_, std::string pathShadow_);
        bool CreateAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string name_, std::string path_);
        bool CreateAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string name_, std::string path_, std::string pathShadow_);

        Texture* GetTexture(std::string name_);
        AnimationTexture* GetAnimationTexture(std::string name_);

        TextureManager() = default;
    private:
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures{};
        std::unordered_map<std::string, std::unique_ptr<AnimationTexture>> m_AnimationTextures{};

        TextureManager(const TextureManager& other) = delete;
        TextureManager& operator=(TextureManager& other) = delete;
    };
}
