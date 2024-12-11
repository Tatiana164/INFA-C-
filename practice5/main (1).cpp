#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <cmath>

// ������� ��� ��������� �������
void drawGraph(sf::RenderWindow& window, std::function<float(float)> func, float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray graph(sf::LinesStrip);

    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x); // ���������� �������� �������

        // �������������� ��������� � ��������
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;

        // ���������� ����� � ������ ������
        graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }

    window.draw(graph);
}

// ����������� ����
int determineZone(float x, float y) {
    if (y > x && y > -x) {
        return 2;
    }
    else if (y < x && y > -x) {
        return 3;
    }
    else if (y < x && y < -x) {
        return 1; 
    }
    else if (y > x && y < -x) {
        return 4;
    }
    return 0; // �� ������
}

int main() {
    // �������� ����
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphing application");

    // ���������� ��� �������� ���������������� �����
    sf::CircleShape userPoint(5); // ������ 5 ��������
    userPoint.setFillColor(sf::Color::Yellow);
    bool userPointExists = false; // ���������� ��� �������� ������������� ���������������� �����

    // �������� ������
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return -1;

    // ����� ��� ����������� ��������� �����
    sf::Text coordinatesText("", font, 20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);

    // ��� X � Y
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(0, 300); // ������ ��� X
    xAxis[0].color = sf::Color::White; // ���� ���
    xAxis[1].position = sf::Vector2f(800, 300); // ����� ��� X
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 0); // ������ ��� Y
    yAxis[0].color = sf::Color::White; // ���� ���
    yAxis[1].position = sf::Vector2f(400, 600); // ����� ��� Y
    yAxis[1].color = sf::Color::White;

    // ���������������
    float scaleX = 40.0f;
    float scaleY = 40.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // �������� ����� �����
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // ��������� ������� �����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // �������������� �������� ��������� � "��������������"
                    float mathX = (mousePos.x - 400) / scaleX;
                    float mathY = -(mousePos.y - 300) / scaleY;

                    // ��������� ����� ���������������� �����
                    userPoint.setPosition(mousePos.x - userPoint.getRadius(), mousePos.y - userPoint.getRadius());
                    userPointExists = true; // ��������, ��� ����� ����������

                    // ����������� ����
                    int zone = determineZone(mathX, mathY);

                    // ���������� ������ � ������������ �����
                    coordinatesText.setString("Coordinates: (" + std::to_string(mathX) + ", " + std::to_string(mathY) +
                        ")\nZone: " + std::to_string(zone));
                }
            }
        }

        // ������� ������
        window.clear();

        // ��������� ����
        window.draw(xAxis);
        window.draw(yAxis);

        // ��������� ������� y1 = x
        drawGraph(window, [](float x) { return x; }, -10, 10, scaleX, scaleY, sf::Color::Red);

        // ��������� ������� y2 = -x
        drawGraph(window, [](float x) { return -x; }, -10, 10, scaleX, scaleY, sf::Color::Blue);

        // ��������� ���������������� �����, ���� ��� ����������
        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }

        // ����������� ������ �����
        window.display();
    }

    return 0;
}