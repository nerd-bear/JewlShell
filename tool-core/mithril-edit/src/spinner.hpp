/*
This specific file is part of the Spinners-Plus-Plus Header Library

Original Source: https://github.com/jmslocum/spinners-plus-plus
Original Author: James Slocum @ Audible Inc.

*/

#ifndef _SPINNERS_HPP
#define _SPINNERS_HPP

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <utility>
#include <mutex>
#include <chrono>

namespace jms
{
  class Animation
  {
  public:
    Animation() = delete;
    Animation(const int interval, const std::vector<std::string> &frames)
        : interval{interval}, frames{frames} {}

    Animation &operator=(const Animation &o)
    {
      interval = o.interval;
      frames = o.frames;
      return *this;
    }

    int getInterval() const { return interval; }
    const std::vector<std::string> &getFrames() const { return frames; }

  private:
    int interval;
    std::vector<std::string> frames;
  };

  static Animation classic = Animation(100,
                                       {"-",
                                        "\\",
                                        "|",
                                        "/"});

  static Animation dots = Animation(80,
                                    {"⠋",
                                     "⠙",
                                     "⠹",
                                     "⠸",
                                     "⠼",
                                     "⠴",
                                     "⠦",
                                     "⠧",
                                     "⠇",
                                     "⠏"});

  static Animation arrows = Animation(120,
                                      {"▹▹▹▹▹",
                                       "▸▹▹▹▹",
                                       "▹▸▹▹▹",
                                       "▹▹▸▹▹",
                                       "▹▹▹▸▹",
                                       "▹▹▹▹▸"});

  static Animation arrows_reverse = Animation(120,
                                              {"◃◃◃◃◃",
                                               "◃◃◃◃◂",
                                               "◃◃◃◂◃",
                                               "◃◃◂◃◃",
                                               "◃◂◃◃◃",
                                               "◂◃◃◃◃"});

  static Animation bounce = Animation(80,
                                      {"[    ]",
                                       "[=   ]",
                                       "[==  ]",
                                       "[=== ]",
                                       "[ ===]",
                                       "[  ==]",
                                       "[   =]",
                                       "[    ]",
                                       "[   =]",
                                       "[  ==]",
                                       "[ ===]",
                                       "[====]",
                                       "[=== ]",
                                       "[==  ]",
                                       "[=   ]"});

  static Animation toggle = Animation(250,
                                      {"⊶",
                                       "⊷"});

  static Animation progress = Animation(80,
                                        {"▰▱▱▱▱▱▱",
                                         "▰▰▱▱▱▱▱",
                                         "▰▰▰▱▱▱▱",
                                         "▰▰▰▰▱▱▱",
                                         "▰▰▰▰▰▱▱",
                                         "▰▰▰▰▰▰▱",
                                         "▰▰▰▰▰▰▰",
                                         "▰▱▱▱▱▱▱"});

  enum class FinishedState
  {
    SUCCESS,
    WARNING,
    FAILURE
  };

  class Spinner
  {
  public:
    Spinner() : text{""}, currentAnimation{dots} {}

    Spinner(const std::string &text) : text{text}, currentAnimation{dots} {}

    Spinner(const std::string &text, Animation &animation) : text{text}, currentAnimation{animation} {}

    void setText(const std::string &text)
    {
      this->text = text;
    }

    void setAnimation(Animation &animation)
    {
      this->currentAnimation = animation;
    }

    void start()
    {
      success = false;
      partial = false;
      finished = false;

      this->animationThread = std::thread(std::ref(*this));
    }

    void finish(FinishedState state)
    {
      finished = true;
      if (state == FinishedState::SUCCESS)
      {
        success = true;
      }
      else if (state == FinishedState::WARNING)
      {
        partial = true;
      }

      animationThread.join();
    }

    void finish(FinishedState state, const std::string &text)
    {
      this->text = text;
      this->finish(state);
    }

    void operator()() const
    {
      int currentFrame = 0;
      int totalFrames;
      int interval;
      while (!finished)
      {
        interval = currentAnimation.getInterval();
        totalFrames = currentAnimation.getFrames().size();

        {
          clearLine();
          std::cout << "\r" << currentAnimation.getFrames()[currentFrame] << " " << text;
          std::flush(std::cout);
        }
        currentFrame++;
        currentFrame %= totalFrames;
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
      }

      // Clear the line and get ready for final output
      clearLine();

      if (success)
      {
        std::cout << "\r" << successSymbol << " " << text << std::endl;
      }
      else if (partial)
      {
        std::cout << "\r" << warnSymbol << " " << text << std::endl;
      }
      else
      {
        std::cout << "\r" << failSymbol << " " << text << std::endl;
      }
    }

    Spinner &operator=(Spinner &&o)
    {
      animationThread = std::move(o.animationThread);
      text = std::move(o.text);
      currentAnimation = o.currentAnimation;
      return *this;
    }

  private:
    std::thread animationThread;
    std::string text;
    Animation &currentAnimation;
    bool finished = false;
    bool success = false;
    bool partial = false;

    constexpr static const char *successSymbol = "\33[1;32m✓\33[0;0m";
    constexpr static const char *failSymbol = "\33[1;31m✕\33[0;0m";
    constexpr static const char *warnSymbol = "\33[1;33m⚠\33[0;0m";

    // Assumes ANSI terminal.
    void clearLine() const
    {
      std::cout << "\33[2K";
    }
  };

};

#endif