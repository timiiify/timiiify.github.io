function RotatorToMatrix(rotation) {
  var radPitch = rotation.Pitch * (Math.PI / 180.0);
  var radYaw = rotation.Yaw * (Math.PI / 180.0);
  var radRoll = rotation.Roll * (Math.PI / 180.0);

  var SP = Math.sin(radPitch);
  var CP = Math.cos(radPitch);
  var SY = Math.sin(radYaw);
  var CY = Math.cos(radYaw);
  var SR = Math.sin(radRoll);
  var CR = Math.cos(radRoll);

  var matrix = new Array(16).fill(0);

  matrix[0] = (CP * CY);
  matrix[1] = (CP * SY);
  matrix[2] = (SP);
  matrix[3] = 0;

  matrix[4] = (SR * SP * CY - CR * SY);
  matrix[5] = (SR * SP * SY + CR * CY);
  matrix[6] = (-SR * CP);
  matrix[7] = 0;

  matrix[8] = (-(CR * SP * CY + SR * SY));
  matrix[9] = (CY * SR - CR * SP * SY);
  matrix[10] = (CR * CP);
  matrix[11] = 0;

  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = 0;
  matrix[15] = 1;

  return matrix;
}

function vectorDot(lhs, rhs) {
  return (((lhs.X * rhs.X) + (lhs.Y * rhs.Y)) + (lhs.Z * rhs.Z));
}

function world2Screen(worldLocation, camViewInfo, tempMatrix) {
  // var tempMatrix = RotatorToMatrix(camViewInfo.Rotation);

  var vAxisX = {
    X: tempMatrix[0],
    Y: tempMatrix[1],
    Z: tempMatrix[2]
  };
  var vAxisY = {
    X: tempMatrix[4],
    Y: tempMatrix[5],
    Z: tempMatrix[6]
  };
  var vAxisZ = {
    X: tempMatrix[8],
    Y: tempMatrix[9],
    Z: tempMatrix[10]
  };

  var vDelta = {
    X: worldLocation.X - camViewInfo.Location.X,
    Y: worldLocation.Y - camViewInfo.Location.Y,
    Z: worldLocation.Z - camViewInfo.Location.Z
  };

  var vTransformed = {
    X: vectorDot(vDelta, vAxisY),
    Y: vectorDot(vDelta, vAxisZ),
    Z: vectorDot(vDelta, vAxisX)
  };

  if (vTransformed.Z < 1.0) {
    vTransformed.Z = 1.0;
  }

  var fov = camViewInfo.FOV;
  var screenCenterX = (sWidth / 2.0);
  var screenCenterY = (sHeight / 2.0);

  var re = {
    X: (screenCenterX + vTransformed.X * (screenCenterX / Math.tan(fov * (Math.PI / 360.0))) /
      vTransformed
        .Z),
    Y: (screenCenterY - vTransformed.Y * (screenCenterX / Math.tan(fov * (Math.PI / 360.0))) /
      vTransformed
        .Z)
  };

  return re;
}
