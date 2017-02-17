#ifndef CLASS_ANIMATION
#define CLASS_ANIMATION

#include <cstdlib>
#include <string>

#include <SFML\Graphics.hpp>

struct AnimationFrame
{
    int m_sprite_index;
    int m_time;
};

class Animation
{
private:

    std::string m_name;

    std::vector<AnimationFrame> m_animation_frames;
    int m_current_frame;
    sf::Clock m_animation_clock;

public:
    Animation(std::string nname = "unnamed");
    ~Animation();

    void offsetFrames(int offset);

    bool loadFromFile(std::string filename);

    std::string getName() { return m_name;}
    void setName(std::string nname) { m_name = nname;}

    int frameCount() { return int(m_animation_frames.size()); }

    int getCurrentFrameIndex() { return m_current_frame;}
    bool setCurrentFrameIndex(int tframe);

    // this should be const
    AnimationFrame *getCurrentAnimationFrame();

    int getCurrentAnimationSpriteIndex();

    void addAnimationFrame(int spriteindex, int ttime = 50);

    void update();

    void show();
};
#endif // CLASS_ANIMATION
