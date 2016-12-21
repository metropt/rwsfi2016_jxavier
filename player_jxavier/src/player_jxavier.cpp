/* _________________________________
   |                                 |
   |           INCLUDES              |
   |_________________________________| */
#include <ros/ros.h>
#include <rwsfi2016_libs/player.h>
#include <math.h>

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

    if( playerToKill.compare("") == 0 )
      playerToKill = msg.green_alive.at(0);

    int tmp = 0;
    for(int i=0;i<msg.green_alive.size();i++){
      if(playerToKill.compare(msg.green_alive.at(i)) != 0){
        tmp = 1;
      }
    }

    if(tmp == 1)
      playerToKill = msg.green_alive.at(0);

    tmp = 0;
    int dist = getDistanceToPlayer(hunters_team->players.at(0));
    for(int i=1;i<hunters_team->players.size();i++){
      if( getDistanceToPlayer(hunters_team->players.at(i)) < dist ){
        dist = getDistanceToPlayer(hunters_team->players.at(i));
        tmp = i;
      }
    }

    cout << getAngleToPLayer("map") << endl;
    if( getDistanceToPlayer("map") > 7.5 && (getAngleToPLayer("map") > 0.75 || getAngleToPLayer("map") < -0.75 ) )
    {
      playerToKill = msg.green_alive.at(msg.green_alive.size()-1);
      if (msg.max_displacement > 0.25)
        move(0.25, getAngleToPLayer("map")  );
      else
        move(msg.max_displacement, getAngleToPLayer("map")  );
      return;
    }


    if(dist < 2.5)
      move(msg.max_displacement, getAngleToPLayer(playerToKill) * -1 );
    else
      move(msg.max_displacement, getAngleToPLayer(playerToKill)  );
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
