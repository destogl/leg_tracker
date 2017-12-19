
#include <iirob_filters/kalman_filter.h>
#include <pcl/point_types.h>

class Leg
{
	
// const static int z_coord = 0.178;
	
	
private:
  int peopleId;
  iirob_filters::MultiChannelKalmanFilter<double>* filter;
  pcl::PointXYZ pos;
  int predictions;
  int observations;
  bool hasPair_;
  int min_predictions;
  int min_observations;
//   bool isRemoved;

public:
  Leg(int min_preds, int min_obs) : min_predictions(min_preds), min_observations(min_obs) {}
  bool configure(pcl::PointXYZ p)
  {
    peopleId = -1;
    pos = p;
    hasPair_ = false;
    std::vector<double> in;
    
    // position
    in.push_back(p.x); in.push_back(p.y);
    // velocity
    in.push_back(0.0); in.push_back(0.0);
    // acceleration
    in.push_back(0.0); in.push_back(0.0);
    
    filter = new iirob_filters::MultiChannelKalmanFilter<double>();
    bool result = filter->configure(in);
    if (!result) { ROS_ERROR("Configure of filter has failed!"); }
    return result;
  }
  pcl::PointXYZ computePrediction()
  {
    std::vector<double> prediction;
    filter->computePrediction(prediction);
    pcl::PointXYZ p;
    if (prediction.size() >= 2) { p.x = prediction[0]; p.y = prediction[1]; p.z = 0.0; }
    return p;
  }
  
  void predict()
  {
    std::vector<double> prediction;
    filter->predict(prediction);
    if (prediction.size() >= 2) { pos.x = prediction[0]; pos.y = prediction[1]; }
    if (predictions < min_predictions) { predictions++; }
    observations = 0;
  }
  
  void update(const std::vector<double>& in, std::vector<double>& out)
  {
    filter->update(in, out);
    if (out.size() < 2) { ROS_ERROR("Update out vector size is too small!"); return; }
    pos.x = out[0];
    pos.y = out[1];
    predictions = 0;
    if (observations < min_observations) { observations++; }
  }
  
  int getPredictions()
  {
    return predictions;
  }
  
  pcl::PointXYZ getPos()
  {
    return pos;
  }
  
  int getPeopleId()
  {
	  return peopleId;
  }
  
  int getObservations()
  {
	  return observations;
  }
  
  void setPeopleId(int id)
  {
	  peopleId = id;
  }
  
  void setHasPair(bool value)
  {
	  hasPair_ = value;
  }
  
  bool hasPair()
  {
	  return hasPair_;
  }
    
};






