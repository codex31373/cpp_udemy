#include <iostream>
#include <string>

//Subsystem classes
struct Projector
{
    void on() { std::cout << "Projector is on" << std::endl; }
    void wide_screen_mode() { std::cout << "Projector in widescreen mode" << std::endl; }
    void off() { std::cout << "Projector is off" << std::endl; }
};

struct SoundSystem
{
    void on() { std::cout << "Sound system is on" << std::endl; }
    void set_volume(int level) { std::cout << "Volume set to " << level << std::endl; }
    void set_surround_sound() { std::cout << "Surround sound enabled" << std::endl; }
    void off() { std::cout << "Sound system is off" << std::endl; }
};

struct DVDPlayer
{
    void on() { std::cout << "DVD player is on" << std::endl; }
    void play(const std::string& movie) { std::cout << "Playing \"" << movie << "\"" << std::endl; }
    void stop() { std::cout << "DVD player stopped" << std::endl; }
    void off() { std::cout << "DVD player is off" << std::endl; }
};

struct Lights
{
    void dim(int level) { std::cout << "Lights dimmed to " << level << "%" << std::endl; }
    void on() { std::cout << "Lights are on" << std::endl; }
};

struct Screen
{
    void down() { std::cout << "Screen is down" << std::endl; }
    void up() { std::cout << "Screen is up" << std::endl; }
};

//Facade
struct HomeTheaterFacade
{
    Projector& projector;
    SoundSystem& sound;
    DVDPlayer& dvd;
    Lights& lights;
    Screen& screen;

    HomeTheaterFacade(Projector& projector, SoundSystem& sound,
                      DVDPlayer& dvd, Lights& lights, Screen& screen)
        : projector(projector), sound(sound), dvd(dvd), lights(lights), screen(screen) {}

    void watch_movie(const std::string& movie)
    {
        std::cout << "\n--- Get ready to watch a movie ---" << std::endl;
        lights.dim(10);
        screen.down();
        projector.on();
        projector.wide_screen_mode();
        sound.on();
        sound.set_surround_sound();
        sound.set_volume(5);
        dvd.on();
        dvd.play(movie);
    }

    void end_movie()
    {
        std::cout << "\n--- Shutting down the home theater ---" << std::endl;
        dvd.stop();
        dvd.off();
        sound.off();
        projector.off();
        screen.up();
        lights.on();
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nFacade : 12 April 2026\n"
              << "\033[0m" << std::endl;

    Projector projector;
    SoundSystem sound;
    DVDPlayer dvd;
    Lights lights;
    Screen screen;

    HomeTheaterFacade home_theater(projector, sound, dvd, lights, screen);

    home_theater.watch_movie("The Matrix");
    home_theater.end_movie();

    return 0;
}