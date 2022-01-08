#include "Demo.h"
#include "imgui.h"
#include "imgui-SFML.h"
Demo::Demo(int sWidth, int sHeight)
{
	// Initializing the window and the different needed attributes
	m_window = new sf::RenderWindow(sf::VideoMode(sWidth, sHeight), "Conway's Game of Life!");
	m_event = new sf::Event;

	// The view size needs to match the window size
	m_view.setSize(sf::Vector2f(m_window->getSize()));

	// Then it needs to be centered correctly
	m_view.setCenter(m_view.getSize() / 2.0f);

	// Grid dimensions
	gHeight = 50;
	gWidth = 50;

	// Movement speed of the view when scrolling
	m_fSpeed = 250.f;
	m_fCellSize = {30.0f, 30.0f};
	m_fOffset = {0.0f, 0.0f};

	m_gridShapes = new sf::RectangleShape[gWidth * gHeight];
	for (int x = 0; x < gWidth; x++)
		for (int y = 0; y < gHeight; y++)
		{
			int index = y * gWidth + x;
			m_gridShapes[index].setSize(m_fCellSize);
			m_gridShapes[index].setOutlineColor(sf::Color(30, 30, 30));
			m_gridShapes[index].setOutlineThickness(2.f);
			m_gridShapes[index].setPosition(x * m_fCellSize.x, y * m_fCellSize.y);
		}

	m_grid = new Grid(gWidth, gHeight);

	// Borders to know if the grid has finished
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
			borders[i].setSize(sf::Vector2f(gWidth * 30.f, 2.f));
		else
			borders[i].setSize(sf::Vector2f(2.f, gHeight * 30.f));
		borders[i].setFillColor(sf::Color::Red);
	}

	borders[0].setPosition(sf::Vector2f(0.0f, 0.0f));
	borders[1].setPosition(sf::Vector2f(0.0f, 0.0f));
	borders[2].setPosition(sf::Vector2f(0.0f, gHeight * m_fCellSize.y - 2.0f));
	borders[3].setPosition(sf::Vector2f(gWidth * m_fCellSize.x - 2.0f, 0.0f));
	ImGui::SFML::Init(*m_window);
}

void Demo::Run()
{
	while (m_window->isOpen())
	{
		EventHandler();
		Update();
		ImGuiLayer();
		Draw();
	}
}

void Demo::EventHandler()
{
	while (m_window->pollEvent(*m_event))
	{
		ImGui::SFML::ProcessEvent(*m_event);
		if (m_event->type == sf::Event::Closed)
			m_window->close();
		if (m_event->type == sf::Event::Resized)
		{
			m_view.setSize(sf::Vector2f(m_window->getSize()));
		}
		if (m_event->type == sf::Event::MouseButtonPressed && m_event->mouseButton.button == sf::Mouse::Left)
		{
			// To know which cell we are looking at
			auto mousePos = sf::Mouse::getPosition(*m_window);

			int x = (mousePos.x / m_fCellSize.x) + (m_fOffset.x / m_fCellSize.x);
			int y = (mousePos.y / m_fCellSize.y) + (m_fOffset.y / m_fCellSize.y);

			auto c = m_grid->GetCell(x, y);

			if (c)
			{
				c->alive = !c->alive;
			}
		}
		if (m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::Space)
		{
			// An indicator that tells if the gird started moving or not
			if (m_grid->IsUpdating())
			{
				m_grid->Stop();
				std::cout << "Stopped\n";
			}
			else
			{
				m_grid->Start();
				std::cout << "Started\n";
			}
		}
	}
}

void Demo::Update()
{
	sf::Time dt = m_clock.restart();
	float deltaTime = dt.asSeconds();
	m_fElapsedTime += deltaTime;
	ImGui::SFML::Update(*m_window, dt);

	// Direction of movment dedection in real time outside events ( to make it have faster response)
	sf::Vector2f direction = {0.0f, 0.0f};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		direction = {-1.0f, 0.0f};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		direction = {+1.0f, 0.0f};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		direction = {0.0f, 1.0f};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		direction = {0.0f, -1.0f};

	m_view.move(direction * m_fSpeed * deltaTime);

	// Keeping track of the top left corner of the view
	m_fViewPos = m_view.getCenter() - (m_view.getSize() / 2.f); // top left corner

	if (m_fViewPos.x < 0.f)
		m_view.setCenter(m_view.getSize().x / 2.0f, m_view.getCenter().y);
	if (m_fViewPos.y < 0.f)
		m_view.setCenter(m_view.getCenter().x, m_view.getSize().y / 2.f);
	if (m_fViewPos.x + m_view.getSize().x > gWidth * m_fCellSize.x)
		m_view.setCenter(gWidth * m_fCellSize.x - m_view.getSize().x / 2.0f, m_view.getCenter().y);
	if (m_fViewPos.y + m_view.getSize().y > gHeight * m_fCellSize.y)
		m_view.setCenter(m_view.getCenter().x, gHeight * m_fCellSize.y - m_view.getSize().y / 2.f);

	m_fViewPos = m_view.getCenter() - (m_view.getSize() / 2.f); // top left corner
	m_fOffset = m_fViewPos;

	// Updating the grid every interval (0.4 seconds)
	if (m_fElapsedTime > 0.4f)
	{
		m_fElapsedTime = 0.f;
		m_grid->Update();
	}
}

void Demo::ImGuiLayer()
{
	ImGui::Begin("Game Settings");
	static bool updating = false;
	if (ImGui::Button("Toggle Simulation"))
	{
		if (updating)
		{
			m_grid->Stop();
			updating = false;
		}
		else
		{
			m_grid->Start();
			updating = true;
		}
	}
	ImGui::Separator();
	ImGui::Text("Alive cells: %d", m_AliveCount);

	ImGui::End();
}

void Demo::Draw()
{
	// Drawing everything onto the screen
	m_window->setView(m_view);
	m_window->clear();
	m_AliveCount = 0;

	for (int x = 0; x < gWidth; x++)
	{
		for (int y = 0; y < gHeight; y++)
		{
			int index = y * gWidth + x;
			if (m_grid->GetCell(x, y)->alive)
			{
				m_gridShapes[index].setFillColor(sf::Color::Black);
				m_AliveCount++;
			}
			else
				m_gridShapes[index].setFillColor(sf::Color(200, 200, 200));
			m_window->draw(m_gridShapes[index]);
		}
	}
	for (int i = 0; i < 4; i++)
		m_window->draw(borders[i]);

	ImGui::SFML::Render(*m_window);
	m_window->display();
}
