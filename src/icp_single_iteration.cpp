#include "icp_single_iteration.h"
#include "random_points_on_mesh.h"
#include "point_mesh_distance.h"
#include "point_to_plane_rigid_matching.h"
#include "point_to_point_rigid_matching.h"

void icp_single_iteration(
  const Eigen::MatrixXd & VX,
  const Eigen::MatrixXi & FX,
  const Eigen::MatrixXd & VY,
  const Eigen::MatrixXi & FY,
  const int num_samples,
  const ICPMethod method,
  Eigen::Matrix3d & R,
  Eigen::RowVector3d & t)
{

    auto transform = [&R, &t](Eigen::MatrixXd& X) {
        return (X * R.transpose()).rowwise() + t;
    };
    
    // Sample X, project X to mesh Y after transformation
    //      P = P_Y(T(X)) \subset Y
    Eigen::VectorXd D;
    Eigen::MatrixXd X, P, N;
    random_points_on_mesh(num_samples,VX,FX,X);
    point_mesh_distance(X, VY, FY, D, P, N);

    // Update rigid transformation to match X and P
    R = Eigen::Matrix3d::Identity();
    t = Eigen::RowVector3d::Zero();
    if (method == ICP_METHOD_POINT_TO_POINT) {
        point_to_point_rigid_matching(X, P, R, t);
    } else if (method == ICP_METHOD_POINT_TO_PLANE) {
        point_to_plane_rigid_matching(X, P, N, R, t);
    }
}
