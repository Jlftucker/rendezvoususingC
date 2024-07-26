#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <iostream>


int main() {
    std::string s = "Boost Libraries";
    std::vector<std::string> words;
    boost::split(words, s, boost::is_any_of(" "));
    for (const auto& word : words) {
        std::cout << word << std::endl;
    }
    return 0;
}

