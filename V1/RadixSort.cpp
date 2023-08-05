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

// Find the maximum value in the vector (used in Radix Sort)
int getMax(std::vector<int>& arr) {
    int max = arr[0];
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

// Counting Sort used in Radix Sort
void countingSort(std::vector<int>& arr, int exp, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> count(10, 0);

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
        drawBars(window, arr, barColor, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
    }
}

// Radix Sort algorithm (updated to enable smooth animation)
void radixSort(std::vector<int>& arr, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    int max = getMax(arr);

    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSort(arr, exp, window, barColor, sortingSpeed);
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

        // Sort the bars using Radix Sort
        radixSort(bars, window, barColor, sortingSpeed);
    }

    return 0;
}
