#include "scene.h"

Scene::Scene(string filename) {
    this->filename = filename;
    this->playableType = 's';
    this->cameraRect = sf::Rect<int>(0, 0, WX, WY);
    this->scrollLeftRect = sf::Rect<int>(0, 0, 200, WY);
    this->scrollRightRect = sf::Rect<int>(WX-200, 0, 200, WY);

    this->load();
}

Scene::~Scene() {
    //dtor
    for (SceneLayer* layer : this->layers) {
        delete layer;
    }
}

void Scene::load() {
    // Do something, Taipu
    // You wrote a system for doing this in Python.  Copy it over.
    // Actually, on second thought, the Python system... can be improved.  I think we can do without the Layer class we had before.
    cout << "Loading scene\n";
    ifstream textFile;
    textFile.open("Data\\Scenes\\" + filename + "\\scene.txt");  // Name the definition files scene.txt
    string line;
    vector<string> splitLine;
    //char loading = '0';  // I guess?

    while (getline(textFile, line)) {
        //cout << line << '\n';
        // DO SOMETHING, TAIPU
        splitLine = splitString(line, ':');
        if (startsWith(line, "dimensions:")) {
            // Get dimensions
            this->dimensions.x = stoi(splitLine[1]);
            this->dimensions.y = stoi(splitLine[2]);
            cout << "X dimensions: " << this->dimensions.x << '\n';
            cout << "Y dimensions: " << this->dimensions.y << '\n';
        }
        if (startsWith(line, "add_layer:")) {
            SceneLayer* newLayer = new SceneLayer("Data\\Scenes\\" + this->filename + "\\Graphics\\" + splitLine[1]);
            newLayer->scrollRateX = stoi(splitLine[2]);
            newLayer->scrollRateY = stoi(splitLine[3]);
            this->layers.push_back(newLayer);
        }
        if (startsWith(line, "add_entity:")) {
            SceneEntity newEntity = SceneEntity(this->filename, splitLine[1]);
            int layerNum = stoi(splitLine[2]);
            this->layers[layerNum]->entities.push_back(newEntity);
        }
    }
    cout << "Finished loading scene\n";
}

void Scene::setHUD(InputBox* hud) {
    this->hud = hud;
}

void Scene::setPlayer(Player* player) {
    this->player = player;
}

void Scene::render(sf::RenderWindow* window) {
    // Do something, Taipu
    /* // This loop renders layers[0] in the back and layers[i] in the front.
    for (SceneLayer* layer : this->layers) {
        layer->render(window);
    }
    */
    // This loop renders layers[0] in the front and layers[i] in the back, which is how the Python worked.  Not sure which is better.
    for (int i=(this->layers.size() - 1); i>=0; i--) {
        this->layers[i]->render(window);
    }
}

string Scene::play(sf::RenderWindow* window) {
    cout << "Called Scene::play()\n";
    // Center the camera rect
    this->cameraRect.left = ((this->dimensions.x / 2) - (WX / 2));
    this->cameraRect.top = ((this->dimensions.y /2) - (WY - 2));

    bool clicked;

    // Main loop
    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        // Here goes with scrolling, moment of truth
        if (this->scrollLeftRect.contains(this->mousePos)) {
            //cout << "Scrolling left\n";
            this->scroll(sf::Vector2<int>(-1, 0));
        }
        if (this->scrollRightRect.contains(this->mousePos)) {
            //cout << "Scrolling right\n";
            this->scroll(sf::Vector2<int>(1, 0));
        }
        // Sweet, now we just need code to deal with entities

        this->render(window);
        window->display();
    }

    return "quit:";
}

void Scene::scroll(sf::Vector2i deltaV) {
    if ((this->cameraRect.left + deltaV.x) < 0 || (this->cameraRect.left + this->cameraRect.width + deltaV.x) > this->dimensions.x) {  // If we don't have room to move horizontally
        //cout << "Setting deltaX to 0\n";
        deltaV.x = 0;  // Set the horizontal movement to 0
    }
    if ((this->cameraRect.top + deltaV.y) < 0 || (this->cameraRect.top + this->cameraRect.height + deltaV.y) > this->dimensions.y) {  // If we don't have room to move vertically
        //cout << "Setting deltaY to 0\n";
        deltaV.y = 0;  // Set the vertical movement to 0
    }
    for (SceneLayer* layer : this->layers) {
        layer->scroll(deltaV);
    }
    this->cameraRect.left += deltaV.x;
    this->cameraRect.top += deltaV.y;
}
