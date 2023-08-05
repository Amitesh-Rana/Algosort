#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

const int windowWidth = 800;
const int windowHeight = 600;
const int barWidth = 5;

// Function to draw bars on the window with custom color
void drawBars(sf::RenderWindow& window, std::vector<int>& bars, sf::Color barColor,
    int comparingIdx = -1, int swappingIdx = -1) {
    window.clear(sf::Color(240, 240, 240)); // Light gray background

    for (size_t i = 0; i < bars.size(); i++) {
        sf::RectangleShape bar(sf::Vector2f(barWidth, bars[i]));
        bar.setPosition(i * barWidth, windowHeight - bars[i]);
        if (i == comparingIdx || i == swappingIdx) {
            bar.setFillColor(sf::Color::Magenta); // Highlight comparing or swapping bars
        }
        else {
            bar.setFillColor(barColor);
        }
        window.draw(bar);
    }

    window.display();
}

// Insertion Sort algorithm (updated to enable smooth animation)
void insertionSort(std::vector<int>& bars, sf::RenderWindow& window, sf::Color barColor,
    int sortingSpeed) {
    for (size_t i = 1; i < bars.size(); i++) {
        int key = bars[i];
        size_t j = i;
        while (j > 0 && bars[j - 1] > key) {
            bars[j] = bars[j - 1];
            drawBars(window, bars, barColor, j, j - 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
            j--;
        }
        bars[j] = key;
        drawBars(window, bars, barColor, j, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sorting Visualizer");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, windowHeight);

    std::vector<int> bars(windowWidth / barWidth);
    for (size_t i = 0; i < bars.size(); i++) {
        bars[i] = dis(gen);
    }

    // Set initial color and sorting speed
    sf::Color barColor = sf::Color::Cyan;
    int sortingSpeed = 15;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update window
        drawBars(window, bars, barColor);

        // Sort the bars using Insertion Sort
        insertionSort(bars, window, barColor, sortingSpeed);
    }

    return 0;
}
