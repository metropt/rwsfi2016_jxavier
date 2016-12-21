/* _________________________________
   |                                 |
   |           INCLUDES              |
   |_________________________________| */
#include <ros/ros.h>
#include <rwsfi2016_libs/player.h>

/* _________________________________
   |                                 |
   |              CODE               |
   |_________________________________| */
using namespace std;
using namespace ros;

string playerToKill;


/**
 * @brief MyPlayer extends class Player, i.e., there are additional things I can do with MyPlayer and not with any Player, e.g., to order a movement.
 */
class MyPlayer: public rwsfi2016_libs::Player
{
public:

  /**
     * @brief Constructor, nothing to be done here
     * @param name player name
     * @param pet_name pet name
     */
  MyPlayer(string player_name, string pet_name="/dog"): Player(player_name, pet_name){};

  void play(const rwsfi2016_msgs::MakeAPlay& msg)
  {
    //Custom play behaviour. Now I will win the game
    string tmp;
    if(playerToKill.compare("")==0)
      playerToKill = msg.green_alive.at(0);

    int test = 0;
    for(int i=0;i<3;i++){
      if(playerToKill.compare(msg.green_alive.at(i)) == 0){
        test = 1;
      }
    }

    if(!test)
      playerToKill = msg.green_alive.at(0);

    //Behaviour follow the closest prey
    move(msg.max_displacement, getAngleToPLayer(playerToKill)  );
    //cout << ((((double)rand()/(double)RAND_MAX) ) * 2 -1) << endl;
  }
};


/**
 * @brief The main function. All you need to do here is enter your name and your pets name
 * @param argc number of command line arguments
 * @param argv values of command line arguments
 * @return result
 */
int main(int argc, char** argv)
{
  // ------------------------
  //Replace this with your name
  // ------------------------
  string my_name = "jxavier";
  string my_pet = "/turtle";
  playerToKill = "";

  //initialize ROS stuff
  ros::init(argc, argv, my_name);

  //Creating an instance of class MyPlayer
  MyPlayer my_player(my_name, my_pet);

  //Infinite spinning (until ctrl-c)
  ros::spin();
}
