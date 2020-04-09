//#line 2 "/opt/ros/melodic/share/dynamic_reconfigure/templates/ConfigType.h.template"
// *********************************************************
//
// File autogenerated for the deplacement package
// by the dynamic_reconfigure package.
// Please do not edit.
//
// ********************************************************/

#ifndef __deplacement__ROBOTCONFIG_H__
#define __deplacement__ROBOTCONFIG_H__

#if __cplusplus >= 201103L
#define DYNAMIC_RECONFIGURE_FINAL final
#else
#define DYNAMIC_RECONFIGURE_FINAL
#endif

#include <dynamic_reconfigure/config_tools.h>
#include <limits>
#include <ros/node_handle.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <dynamic_reconfigure/ParamDescription.h>
#include <dynamic_reconfigure/Group.h>
#include <dynamic_reconfigure/config_init_mutex.h>
#include <boost/any.hpp>

namespace deplacement
{
  class RobotConfigStatics;

  class RobotConfig
  {
  public:
    class AbstractParamDescription : public dynamic_reconfigure::ParamDescription
    {
    public:
      AbstractParamDescription(std::string n, std::string t, uint32_t l,
          std::string d, std::string e)
      {
        name = n;
        type = t;
        level = l;
        description = d;
        edit_method = e;
      }

      virtual void clamp(RobotConfig &config, const RobotConfig &max, const RobotConfig &min) const = 0;
      virtual void calcLevel(uint32_t &level, const RobotConfig &config1, const RobotConfig &config2) const = 0;
      virtual void fromServer(const ros::NodeHandle &nh, RobotConfig &config) const = 0;
      virtual void toServer(const ros::NodeHandle &nh, const RobotConfig &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, RobotConfig &config) const = 0;
      virtual void toMessage(dynamic_reconfigure::Config &msg, const RobotConfig &config) const = 0;
      virtual void getValue(const RobotConfig &config, boost::any &val) const = 0;
    };

    typedef boost::shared_ptr<AbstractParamDescription> AbstractParamDescriptionPtr;
    typedef boost::shared_ptr<const AbstractParamDescription> AbstractParamDescriptionConstPtr;

    // Final keyword added to class because it has virtual methods and inherits
    // from a class with a non-virtual destructor.
    template <class T>
    class ParamDescription DYNAMIC_RECONFIGURE_FINAL : public AbstractParamDescription
    {
    public:
      ParamDescription(std::string a_name, std::string a_type, uint32_t a_level,
          std::string a_description, std::string a_edit_method, T RobotConfig::* a_f) :
        AbstractParamDescription(a_name, a_type, a_level, a_description, a_edit_method),
        field(a_f)
      {}

      T (RobotConfig::* field);

      virtual void clamp(RobotConfig &config, const RobotConfig &max, const RobotConfig &min) const
      {
        if (config.*field > max.*field)
          config.*field = max.*field;

        if (config.*field < min.*field)
          config.*field = min.*field;
      }

      virtual void calcLevel(uint32_t &comb_level, const RobotConfig &config1, const RobotConfig &config2) const
      {
        if (config1.*field != config2.*field)
          comb_level |= level;
      }

      virtual void fromServer(const ros::NodeHandle &nh, RobotConfig &config) const
      {
        nh.getParam(name, config.*field);
      }

      virtual void toServer(const ros::NodeHandle &nh, const RobotConfig &config) const
      {
        nh.setParam(name, config.*field);
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, RobotConfig &config) const
      {
        return dynamic_reconfigure::ConfigTools::getParameter(msg, name, config.*field);
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const RobotConfig &config) const
      {
        dynamic_reconfigure::ConfigTools::appendParameter(msg, name, config.*field);
      }

      virtual void getValue(const RobotConfig &config, boost::any &val) const
      {
        val = config.*field;
      }
    };

    class AbstractGroupDescription : public dynamic_reconfigure::Group
    {
      public:
      AbstractGroupDescription(std::string n, std::string t, int p, int i, bool s)
      {
        name = n;
        type = t;
        parent = p;
        state = s;
        id = i;
      }

      std::vector<AbstractParamDescriptionConstPtr> abstract_parameters;
      bool state;

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &config) const =0;
      virtual void updateParams(boost::any &cfg, RobotConfig &top) const= 0;
      virtual void setInitialState(boost::any &cfg) const = 0;


      void convertParams()
      {
        for(std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = abstract_parameters.begin(); i != abstract_parameters.end(); ++i)
        {
          parameters.push_back(dynamic_reconfigure::ParamDescription(**i));
        }
      }
    };

    typedef boost::shared_ptr<AbstractGroupDescription> AbstractGroupDescriptionPtr;
    typedef boost::shared_ptr<const AbstractGroupDescription> AbstractGroupDescriptionConstPtr;

    // Final keyword added to class because it has virtual methods and inherits
    // from a class with a non-virtual destructor.
    template<class T, class PT>
    class GroupDescription DYNAMIC_RECONFIGURE_FINAL : public AbstractGroupDescription
    {
    public:
      GroupDescription(std::string a_name, std::string a_type, int a_parent, int a_id, bool a_s, T PT::* a_f) : AbstractGroupDescription(a_name, a_type, a_parent, a_id, a_s), field(a_f)
      {
      }

      GroupDescription(const GroupDescription<T, PT>& g): AbstractGroupDescription(g.name, g.type, g.parent, g.id, g.state), field(g.field), groups(g.groups)
      {
        parameters = g.parameters;
        abstract_parameters = g.abstract_parameters;
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        if(!dynamic_reconfigure::ConfigTools::getGroupState(msg, name, (*config).*field))
          return false;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          if(!(*i)->fromMessage(msg, n))
            return false;
        }

        return true;
      }

      virtual void setInitialState(boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        T* group = &((*config).*field);
        group->state = state;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = boost::any(&((*config).*field));
          (*i)->setInitialState(n);
        }

      }

      virtual void updateParams(boost::any &cfg, RobotConfig &top) const
      {
        PT* config = boost::any_cast<PT*>(cfg);

        T* f = &((*config).*field);
        f->setParams(top, abstract_parameters);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          (*i)->updateParams(n, top);
        }
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &cfg) const
      {
        const PT config = boost::any_cast<PT>(cfg);
        dynamic_reconfigure::ConfigTools::appendGroup<T>(msg, name, id, parent, config.*field);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          (*i)->toMessage(msg, config.*field);
        }
      }

      T (PT::* field);
      std::vector<RobotConfig::AbstractGroupDescriptionConstPtr> groups;
    };

class DEFAULT
{
  public:
    DEFAULT()
    {
      state = true;
      name = "Default";
    }

    void setParams(RobotConfig &config, const std::vector<AbstractParamDescriptionConstPtr> params)
    {
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator _i = params.begin(); _i != params.end(); ++_i)
      {
        boost::any val;
        (*_i)->getValue(config, val);

        if("speed"==(*_i)->name){speed = boost::any_cast<int>(val);}
        if("max_distance"==(*_i)->name){max_distance = boost::any_cast<int>(val);}
        if("duration"==(*_i)->name){duration = boost::any_cast<int>(val);}
        if("rotation"==(*_i)->name){rotation = boost::any_cast<int>(val);}
        if("direction"==(*_i)->name){direction = boost::any_cast<int>(val);}
        if("action"==(*_i)->name){action = boost::any_cast<std::string>(val);}
        if("launch"==(*_i)->name){launch = boost::any_cast<bool>(val);}
      }
    }

    int speed;
int max_distance;
int duration;
int rotation;
int direction;
std::string action;
bool launch;

    bool state;
    std::string name;

    
}groups;



//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      int speed;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      int max_distance;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      int duration;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      int rotation;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      int direction;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      std::string action;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      bool launch;
//#line 228 "/opt/ros/melodic/share/dynamic_reconfigure/templates/ConfigType.h.template"

    bool __fromMessage__(dynamic_reconfigure::Config &msg)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();

      int count = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        if ((*i)->fromMessage(msg, *this))
          count++;

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i ++)
      {
        if ((*i)->id == 0)
        {
          boost::any n = boost::any(this);
          (*i)->updateParams(n, *this);
          (*i)->fromMessage(msg, n);
        }
      }

      if (count != dynamic_reconfigure::ConfigTools::size(msg))
      {
        ROS_ERROR("RobotConfig::__fromMessage__ called with an unexpected parameter.");
        ROS_ERROR("Booleans:");
        for (unsigned int i = 0; i < msg.bools.size(); i++)
          ROS_ERROR("  %s", msg.bools[i].name.c_str());
        ROS_ERROR("Integers:");
        for (unsigned int i = 0; i < msg.ints.size(); i++)
          ROS_ERROR("  %s", msg.ints[i].name.c_str());
        ROS_ERROR("Doubles:");
        for (unsigned int i = 0; i < msg.doubles.size(); i++)
          ROS_ERROR("  %s", msg.doubles[i].name.c_str());
        ROS_ERROR("Strings:");
        for (unsigned int i = 0; i < msg.strs.size(); i++)
          ROS_ERROR("  %s", msg.strs[i].name.c_str());
        // @todo Check that there are no duplicates. Make this error more
        // explicit.
        return false;
      }
      return true;
    }

    // This version of __toMessage__ is used during initialization of
    // statics when __getParamDescriptions__ can't be called yet.
    void __toMessage__(dynamic_reconfigure::Config &msg, const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__, const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__) const
    {
      dynamic_reconfigure::ConfigTools::clear(msg);
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toMessage(msg, *this);

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        if((*i)->id == 0)
        {
          (*i)->toMessage(msg, *this);
        }
      }
    }

    void __toMessage__(dynamic_reconfigure::Config &msg) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      __toMessage__(msg, __param_descriptions__, __group_descriptions__);
    }

    void __toServer__(const ros::NodeHandle &nh) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toServer(nh, *this);
    }

    void __fromServer__(const ros::NodeHandle &nh)
    {
      static bool setup=false;

      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->fromServer(nh, *this);

      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i++){
        if (!setup && (*i)->id == 0) {
          setup = true;
          boost::any n = boost::any(this);
          (*i)->setInitialState(n);
        }
      }
    }

    void __clamp__()
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const RobotConfig &__max__ = __getMax__();
      const RobotConfig &__min__ = __getMin__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->clamp(*this, __max__, __min__);
    }

    uint32_t __level__(const RobotConfig &config) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      uint32_t level = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->calcLevel(level, config, *this);
      return level;
    }

    static const dynamic_reconfigure::ConfigDescription &__getDescriptionMessage__();
    static const RobotConfig &__getDefault__();
    static const RobotConfig &__getMax__();
    static const RobotConfig &__getMin__();
    static const std::vector<AbstractParamDescriptionConstPtr> &__getParamDescriptions__();
    static const std::vector<AbstractGroupDescriptionConstPtr> &__getGroupDescriptions__();

  private:
    static const RobotConfigStatics *__get_statics__();
  };

  template <> // Max and min are ignored for strings.
  inline void RobotConfig::ParamDescription<std::string>::clamp(RobotConfig &config, const RobotConfig &max, const RobotConfig &min) const
  {
    (void) config;
    (void) min;
    (void) max;
    return;
  }

  class RobotConfigStatics
  {
    friend class RobotConfig;

    RobotConfigStatics()
    {
RobotConfig::GroupDescription<RobotConfig::DEFAULT, RobotConfig> Default("Default", "", 0, 0, true, &RobotConfig::groups);
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.speed = -100;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.speed = 100;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.speed = 40;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("speed", "int", 0, "Speed used when calling actions", "", &RobotConfig::speed)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("speed", "int", 0, "Speed used when calling actions", "", &RobotConfig::speed)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.max_distance = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.max_distance = 40;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.max_distance = 20;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("max_distance", "int", 0, "Maximum distance between the robot and an obstacle to be safe", "", &RobotConfig::max_distance)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("max_distance", "int", 0, "Maximum distance between the robot and an obstacle to be safe", "", &RobotConfig::max_distance)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.duration = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.duration = 20;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.duration = 5;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("duration", "int", 0, "Duration of the action", "", &RobotConfig::duration)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("duration", "int", 0, "Duration of the action", "", &RobotConfig::duration)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.rotation = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.rotation = 1;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.rotation = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("rotation", "int", 0, "Used to define the direction of rotation", "{'enum_description': 'Directions of rotation', 'enum': [{'srcline': 14, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'clockwise'}, {'srcline': 15, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'counter_clockwise'}]}", &RobotConfig::rotation)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("rotation", "int", 0, "Used to define the direction of rotation", "{'enum_description': 'Directions of rotation', 'enum': [{'srcline': 14, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'clockwise'}, {'srcline': 15, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'counter_clockwise'}]}", &RobotConfig::rotation)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.direction = -1;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.direction = 1;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.direction = 1;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("direction", "int", 0, "Used to choose the direction where the robot will go", "{'enum_description': 'Directions', 'enum': [{'srcline': 18, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'forward'}, {'srcline': 19, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': -1, 'ctype': 'int', 'type': 'int', 'name': 'reverse'}]}", &RobotConfig::direction)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<int>("direction", "int", 0, "Used to choose the direction where the robot will go", "{'enum_description': 'Directions', 'enum': [{'srcline': 18, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'forward'}, {'srcline': 19, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const int', 'value': -1, 'ctype': 'int', 'type': 'int', 'name': 'reverse'}]}", &RobotConfig::direction)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.action = "";
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.action = "";
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.action = "straight";
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<std::string>("action", "str", 0, "Select the action that the robot will do", "{'enum_description': 'Actions of the robot', 'enum': [{'srcline': 22, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'straight', 'ctype': 'std::string', 'type': 'str', 'name': 'straight'}, {'srcline': 23, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'turn', 'ctype': 'std::string', 'type': 'str', 'name': 'turn'}, {'srcline': 24, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'spin', 'ctype': 'std::string', 'type': 'str', 'name': 'spin'}]}", &RobotConfig::action)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<std::string>("action", "str", 0, "Select the action that the robot will do", "{'enum_description': 'Actions of the robot', 'enum': [{'srcline': 22, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'straight', 'ctype': 'std::string', 'type': 'str', 'name': 'straight'}, {'srcline': 23, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'turn', 'ctype': 'std::string', 'type': 'str', 'name': 'turn'}, {'srcline': 24, 'description': '', 'srcfile': '/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg', 'cconsttype': 'const char * const', 'value': 'spin', 'ctype': 'std::string', 'type': 'str', 'name': 'spin'}]}", &RobotConfig::action)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.launch = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.launch = 1;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.launch = 0;
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<bool>("launch", "bool", 0, "Set to true to launch the action", "", &RobotConfig::launch)));
//#line 274 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(RobotConfig::AbstractParamDescriptionConstPtr(new RobotConfig::ParamDescription<bool>("launch", "bool", 0, "Set to true to launch the action", "", &RobotConfig::launch)));
//#line 246 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.convertParams();
//#line 246 "/opt/ros/melodic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __group_descriptions__.push_back(RobotConfig::AbstractGroupDescriptionConstPtr(new RobotConfig::GroupDescription<RobotConfig::DEFAULT, RobotConfig>(Default)));
//#line 366 "/opt/ros/melodic/share/dynamic_reconfigure/templates/ConfigType.h.template"

      for (std::vector<RobotConfig::AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        __description_message__.groups.push_back(**i);
      }
      __max__.__toMessage__(__description_message__.max, __param_descriptions__, __group_descriptions__);
      __min__.__toMessage__(__description_message__.min, __param_descriptions__, __group_descriptions__);
      __default__.__toMessage__(__description_message__.dflt, __param_descriptions__, __group_descriptions__);
    }
    std::vector<RobotConfig::AbstractParamDescriptionConstPtr> __param_descriptions__;
    std::vector<RobotConfig::AbstractGroupDescriptionConstPtr> __group_descriptions__;
    RobotConfig __max__;
    RobotConfig __min__;
    RobotConfig __default__;
    dynamic_reconfigure::ConfigDescription __description_message__;

    static const RobotConfigStatics *get_instance()
    {
      // Split this off in a separate function because I know that
      // instance will get initialized the first time get_instance is
      // called, and I am guaranteeing that get_instance gets called at
      // most once.
      static RobotConfigStatics instance;
      return &instance;
    }
  };

  inline const dynamic_reconfigure::ConfigDescription &RobotConfig::__getDescriptionMessage__()
  {
    return __get_statics__()->__description_message__;
  }

  inline const RobotConfig &RobotConfig::__getDefault__()
  {
    return __get_statics__()->__default__;
  }

  inline const RobotConfig &RobotConfig::__getMax__()
  {
    return __get_statics__()->__max__;
  }

  inline const RobotConfig &RobotConfig::__getMin__()
  {
    return __get_statics__()->__min__;
  }

  inline const std::vector<RobotConfig::AbstractParamDescriptionConstPtr> &RobotConfig::__getParamDescriptions__()
  {
    return __get_statics__()->__param_descriptions__;
  }

  inline const std::vector<RobotConfig::AbstractGroupDescriptionConstPtr> &RobotConfig::__getGroupDescriptions__()
  {
    return __get_statics__()->__group_descriptions__;
  }

  inline const RobotConfigStatics *RobotConfig::__get_statics__()
  {
    const static RobotConfigStatics *statics;

    if (statics) // Common case
      return statics;

    boost::mutex::scoped_lock lock(dynamic_reconfigure::__init_mutex__);

    if (statics) // In case we lost a race.
      return statics;

    statics = RobotConfigStatics::get_instance();

    return statics;
  }

//#line 14 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const int Robot_clockwise = 0;
//#line 15 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const int Robot_counter_clockwise = 1;
//#line 18 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const int Robot_forward = 1;
//#line 19 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const int Robot_reverse = -1;
//#line 22 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const char * const Robot_straight = "straight";
//#line 23 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const char * const Robot_turn = "turn";
//#line 24 "/home/leo/DaVinciBot-FrenchRoboticsCup-2020/cdr_robot_ws/src/deplacement/cfg/Robot.cfg"
      const char * const Robot_spin = "spin";
}

#undef DYNAMIC_RECONFIGURE_FINAL

#endif // __ROBOTRECONFIGURATOR_H__
