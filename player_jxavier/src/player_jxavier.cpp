/* _________________________________
   |                                 |
   |           INCLUDES              |
   |_________________________________| */
#include <ros/ros.h>
#include <rwsfi2016_libs/player.h>
#include <std_msgs/String.h>
#include <visualization_msgs/Marker.h>
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
  ros::Publisher publisher;
  visualization_msgs::Marker bocas_msg;
  /**
     * @brief Constructor, nothing to be done here
     * @param name player name
     * @param pet_name pet name
     */
  MyPlayer(string player_name, string pet_name="/dog"): Player(player_name, pet_name){
    publisher = node.advertise<visualization_msgs::Marker>("/bocas", 1);
    bocas_msg.header.frame_id = "testeves";
    bocas_msg.ns = "testeves";
    bocas_msg.id = 0;
    bocas_msg.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    bocas_msg.action = visualization_msgs::Marker::ADD;
    bocas_msg.scale.z = 0.4;
    bocas_msg.pose.position.y = -0.4;
    bocas_msg.color.a = 1.0; // Don't forget to set the alpha!
    bocas_msg.color.r = 0.0;
    bocas_msg.color.g = 0.0;
    bocas_msg.color.b = 0.0;
  };

  void play(const rwsfi2016_msgs::MakeAPlay& msg)
  {
    //Custom play behaviour. Now I will win the game

    bocas_msg.header.stamp = ros::Time();
    bocas_msg.text = "SOU UM GATO!";
    publisher.publish(bocas_msg);

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
    if( getDistanceToPlayer("map") > 7.5 && (getAngleToPLayer("map") > 0.5 || getAngleToPLayer("map") < -0.5 ) )
    {
      playerToKill = msg.green_alive.at(msg.green_alive.size()-1);
      if (msg.max_displacement > 0.25)
        move(0.25, getAngleToPLayer("map")  );
      else
        move(msg.max_displacement, getAngleToPLayer("map")  );
      return;
    }


    if(dist < 2.2)
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
