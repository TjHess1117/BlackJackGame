#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    currentX(0),
    currentY(0)
{
    ui->setupUi(this);

    // Setup Box2D for each chip with different x positions
    setupBox2D();

    connect(timer, &QTimer::timeout, this, &MainWindow::updateWorld);
    timer->start(1000 / 60); // 60Hz
}

MainWindow::~MainWindow() {
    delete ui;
    for (auto& world : worlds) {
        delete world;
    }
}

void MainWindow::setupBox2D() {
    // Create Box2D world for each label with different x positions
    float initialX = -10.0f; // Initial x position
    float spacing = 5.0f; // Spacing between bodies
    for (int i = 0; i < 9; ++i) {
        b2Vec2 gravity(0.0f, -50.0f);
        worlds.push_back(new b2World(gravity));
        setupBox2D(initialX + spacing * i, i, i); // Pass the label index as well
    }
}

void MainWindow::setupBox2D(float x, int index, int labelIndex) {
    // Ground body setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = worlds[index]->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Dynamic body setup for the circle
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, 10.0f); // Adjusted initial Y position to prevent glitch
    b2Body* circleBody = worlds[index]->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f; // Adjusted for Box2D's scale

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f; // High restitution for bouncing

    circleBody->CreateFixture(&fixtureDef);

    // Apply initial upward impulse to make it fall
    circleBody->ApplyLinearImpulse(b2Vec2(0.0f, -10.0f), circleBody->GetWorldCenter(), true);

    // Increment index to create the next body, if not reached the limit
    if (++index < 9) {
        float delay = 0.2f; // Delay in seconds
        QTimer::singleShot(static_cast<int>(delay * 1000), [=]() {
            setupBox2D(x + 5.0f, index, labelIndex); // Pass the label index
        });
    }
}


void MainWindow::updateWorld() {
    static float totalTime = 2.0;
    static float elapsedTime = 0.0f;
    elapsedTime += 1.0f / 60.0f; // Increment by 1/60 seconds

    // Apply animation only if within the total animation time
    if (elapsedTime <= totalTime) {
        // Step the Box2D worlds
        for (int i = 0; i < worlds.size(); ++i) {
            worlds[i]->Step(1.0f / 60.0f, 10, 5); // Step the world
        }

        // Update the positions of the QLabel objects
        QPixmap pixmap("/Users/manyanair/Downloads/pokerchip.png"); // Adjust the path to your poker chip image
        QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);

        // Calculate the initial Y position for all labels (top of the window)
        float initialY = 0;

        // Adjust the initial x position and spacing between labels
        float initialX = 10.0f; // Initial x position
        float labelSpacing = 100.0f; // Spacing between labels

        for (int i = 0; i < worlds.size(); ++i) {
            QLabel *label = nullptr;
            switch (i) {
            case 0: label = ui->label; break;
            case 1: label = ui->label_2; break;
            case 2: label = ui->label_3; break;
            case 3: label = ui->label_4; break;
            case 4: label = ui->label_5; break;
            case 5: label = ui->label_6; break;
            case 6: label = ui->label_7; break;
            case 7: label = ui->label_8; break;
            case 8: label = ui->label_9; break;
            default: break;
            }

            if (label) {
                // Get the Box2D body position for the current label
                b2Body* body = worlds[i]->GetBodyList();
                b2Vec2 position;
                if (body) {
                    position = body->GetPosition();
                } else {
                    qDebug() << "No body found for label" << i + 1;
                    continue;
                }

                // Debugging output: Print the position of the Box2D body
                qDebug() << "Label" << i + 1 << "Position:" << position.x << "," << position.y;

                // Map the Box2D Y position to QLabel Y position
                float posY = initialY + (this->height() - scaledPixmap.height()) - position.y * 100; // Adjusted for Box2D scale

                // Map the Box2D X position to QLabel X position
                float posX = initialX + labelSpacing * i; // Adjusted for spacing

                // Calculate the final Y position based on the window's height
                float finalY = this->height() - scaledPixmap.height();
                // Smoothly interpolate the Y position towards the final position
                posY += (finalY - posY) * (elapsedTime / totalTime);

                label->setPixmap(scaledPixmap);
                label->move(posX, posY);
            }
        }
    } else {
        // Animation finished, stop the timer
        timer->stop();
    }
}
