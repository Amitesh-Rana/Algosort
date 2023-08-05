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

// Heapify function used in Heap Sort
void heapify(std::vector<int>& arr, size_t n, size_t i, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        drawBars(window, arr, barColor, i, largest);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
        heapify(arr, n, largest, window, barColor, sortingSpeed);
    }
}

// Heap Sort algorithm (updated to enable smooth animation)
void heapSort(std::vector<int>& arr, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    size_t n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, window, barColor, sortingSpeed);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        drawBars(window, arr, barColor, 0, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
        heapify(arr, i, 0, window, barColor, sortingSpeed);
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

        // Sort the bars using Heap Sort
        heapSort(bars, window, barColor, sortingSpeed);
    }

    return 0;
}
