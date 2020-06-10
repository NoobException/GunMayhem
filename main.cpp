#include <memory>
#include <iostream>
#include <string>
#include <list>
#include <deque>
#include <vector>
#include <queue>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "circular_buffer.hpp"

// TODO:
// Player can walk
// Player shall be displayed on some Window
// Player shall send information via Network

namespace Game
{
  class Network
  {
  public:
    Network();
  };

  class UI
  {
  public:
    UI();
  };

  struct Position
  {
    float x, y;
  };

  struct Player
  {
    Position position;
    std::string name;
  };

  class PlayerController
  {
  public:
    void move_left()
    {
      player.position.x += speed;
    }
    void move_right()
    {
      player.position.x -= speed;
    }

  private:
    Player &player;
    float speed = 0.1f;
  };

  class PlayerRenderer
  {
  public:
    PlayerRenderer(const Player &player) : player(player) {}
    void render(sf::RenderWindow window)
    {
      sprite.setPosition(sf::Vector2f(player.position.x, player.position.y));
      window.draw(sprite);
    }

    void setPlayerTexture(sf::Texture *texture)
    {
      sprite.setTexture(*texture);
    }

  private:
    sf::Sprite sprite;
    const Player &player;
  };

  class Animation
  {
  public:
    Animation(PlayerRenderer &playerRenderer) : playerRenderer(playerRenderer) {}
    void setPlayerTexture()
    {
      playerRenderer.setPlayerTexture(*(textures[animationState]));
    }

  private:
    PlayerRenderer &playerRenderer;
    int animationState;
    std::deque<sf::Texture *> textures;
  };

  class Game
  {
  public:
    const unsigned int FPS = 60;
    Game(std::unique_ptr<UI> &&ui, std::unique_ptr<Network> &&network)
    {
      this->ui = ui;
      this->network = network;
    }
    void run()
    {
      main_loop();
    }

  private:
    void main_loop()
    {
      while (ui->is_running())
      {
        process_ui_events();
      }
    }
    void process_ui_events()
    {
      UI::Event event;
      while (ui.pollEvent(event))
      {
        switch (event.type)
      }
    }
    std::unique_ptr<UI> ui;
    std::unique_ptr<Network> network;
  };

  class Client
  {
  public:
    Player my_player;
    std::list<Player> other_players;
  };

  //poniżej się dzieją śmieszne rzeczy
  //patrzeć na własną odpowiedzialność
  class UnimportantNetworkMessage
  {
  };

  class ImportantNetworkMessage // !important
  {
  };

  class NetworkMessageReceiver
  {
  public:
    NetworkMessageReceiver();

  private:
    sf::Thread receiving_thread;
    unsigned int current_important_message_id;
  };

  class NetworkMessageSender
  {
  public:
    NetworkMessageSender();

  private:
    unsigned int current_important_message_id;
  };

} // namespace Game

enum PlayerAction
{
  MOVE_LEFT,
  MOVE_RIGHT,
  IDLE,
};

class PlayerActions
{
public:
  std::vector<PlayerAction> actions_since(int action_id)
  {
    PlayerAction lastAction = actions.last();
    int action_count = last_action_id - action_id + 1;
    return actions.lastN(action_count);
  };

  void put(PlayerAction action)
  {
    last_action_id++;
    actions.put(action);
  }
  
private:
  int last_action_id;
  CircularBuffer<PlayerAction> actions;

};



struct AddressedPacket
{
  sf::Packet packet;
  sf::IpAddress address;
  unsigned short port;
};

class Socket
{
public:
  Socket(unsigned short port) : receiving_thread(receiving_loop, this)
  {
    socket.bind(port);
    receiving_thread.launch();
  }
  ~Socket()
  {
    receiving_thread.terminate();
  }
  bool has_next_packet()
  {
    queue_mutex.lock();
    bool ans = !packet_queue.empty();
    queue_mutex.unlock();
    return ans;
  }
  AddressedPacket get_next_packet()
  {
    queue_mutex.lock();
    AddressedPacket ans = std::move(packet_queue.front());
    packet_queue.pop();
    queue_mutex.unlock();
    return ans;
  }
  void send_packet(sf::Packet packet, sf::IpAddress address, unsigned short port)
  {
    socket.send(packet, address, port);
  }

private:
  void receiving_loop()
  {
    while (true)
    {
      sf::Packet packet;
      sf::IpAddress address;
      unsigned short port;
      socket.receive(packet, address, port);

      queue_mutex.lock();
      packet_queue.push({packet, address, port});
      queue_mutex.unlock();
    }
  }
  sf::Mutex queue_mutex;
  std::queue<AddressedPacket> packet_queue;
  sf::Thread receiving_thread;
  sf::UdpSocket socket;
};

int main()
{
  Socket s1(25000);
  Socket s2(25001);
  sf::Packet packet;
  sf::IpAddress address("127.0.0.1");
  packet << std::string("Hello");
  s1.send_packet(packet, address, 25001);
  while(!s2.has_next_packet)
  AddressedPacket packet2 = s2.get_next_packet();
  std::string ans;
  packet2.packet >> ans;

  std::cout << ans << std::endl;
  
  
  /*std::unique_ptr<Game::Network> network = make_unique<Game::Network>();
  std::unique_ptr<Game::UI> window = make_unique<Game::UI>();
  
  Game::Game game(move(window), move(network));
  game.run();
  */
  return 0;
}