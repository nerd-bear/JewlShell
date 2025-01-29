#include "spinner.hpp"

#include <thread>
#include <string>
#include <chrono>
#include <sstream>

using namespace std;

int main()
{
    jms::Spinner spinner = jms::Spinner("Attempting to scan all directories", jms::dots);
    spinner.start();

    this_thread::sleep_for(2s);
    // spinner.setAnimation(jms::dots);
    // this_thread::sleep_for(2s);

    // spinner.finish(jms::FinishedState::FAILURE, "Failed to finish that task");
    // this_thread::sleep_for(3s);
    spinner.finish(jms::FinishedState::SUCCESS, "Found 4 results");

    return 0;
}