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

// Merge function used in Merge Sort
void merge(std::vector<int>& arr, size_t l, size_t m, size_t r, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    size_t n1 = m - l + 1;
    size_t n2 = r - m;

    std::vector<int> left(n1);
    std::vector<int> right(n2);

    for (size_t i = 0; i < n1; i++)
        left[i] = arr[l + i];
    for (size_t j = 0; j < n2; j++)
        right[j] = arr[m + 1 + j];

    size_t i = 0;
    size_t j = 0;
    size_t k = l;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            drawBars(window, arr, barColor, k);
            std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
            i++;
        }
        else {
            arr[k] = right[j];
            drawBars(window, arr, barColor, k);
            std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        drawBars(window, arr, barColor, k);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right[j];
        drawBars(window, arr, barColor, k);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortingSpeed));
        j++;
        k++;
    }
}

// Merge Sort algorithm (updated to enable smooth animation)
void mergeSort(std::vector<int>& arr, size_t l, size_t r, sf::RenderWindow& window, sf::Color barColor, int sortingSpeed) {
    if (l < r) {
        size_t m = l + (r - l) / 2;
        mergeSort(arr, l, m, window, barColor, sortingSpeed);
        mergeSort(arr, m + 1, r, window, barColor, sortingSpeed);
        merge(arr, l, m, r, window, barColor, sortingSpeed);
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

        // Sort the bars using Merge Sort
        mergeSort(bars, 0, bars.size() - 1, window, barColor, sortingSpeed);
    }

    return 0;
}
