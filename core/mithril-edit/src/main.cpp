#include "spinner.hpp"

#include <thread>
#include <string>
#include <chrono>
#include <sstream>

using namespace std;

int main()
{
    jms::Spinner s = jms::Spinner("Attempting to scan all directories", jms::dots);
    s.start();

    this_thread::sleep_for(2s);
    // s.setAnimation(jms::dots);
    // this_thread::sleep_for(2s);

    // s.finish(jms::FinishedState::FAILURE, "Failed to finish that task");
    // this_thread::sleep_for(3s);
    s.finish(jms::FinishedState::SUCCESS, "Found 4 results");

    return 0;
}