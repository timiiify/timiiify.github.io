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
function Coordinateto2D(worldPos, camViewInfo, tempMatrix, int) {
		var zb1 = {
			X: worldPos.X,
			Y: worldPos.Y,
			Z: worldPos.Z + int,
		}
		var zb2 = {
			X: worldPos.X,
			Y: worldPos.Y,
			Z: worldPos.Z - int,
		}
		var fkzb1 = world2Screen(zb1, camViewInfo, tempMatrix);
		var fkzb2 = world2Screen(zb2, camViewInfo, tempMatrix);
		var fk = {
			X: fkzb1.X,
			Y: fkzb1.Y,
			G: fkzb2.Y - fkzb1.Y,
			K: (fkzb2.Y - fkzb1.Y) / 2,
		}
		return fk;
	}
	function get3DDis(XYZ, camViewInfo) {
		var distX = (XYZ.X - camViewInfo.Location.X) / 100;
		var distY = (XYZ.Y - camViewInfo.Location.Y) / 100;
		var distance = (distX * distX) + (distY * distY);
		var distZ = (XYZ.Z - camViewInfo.Location.Z) / 100;
		distance = Math.ceil(Math.sqrt((distZ * distZ) + distance));
		return distance;
	}
	function getWorldXYZ(actor ,py1 ,py2) {
		var rootComponent = readLong(actor + py1);
		var worldPos = {
			X: readFloat(rootComponent + py2),
			Y: readFloat(rootComponent + py2 + 4),
			Z: readFloat(rootComponent + py2 + 4 + 4)
		}
		return worldPos;
	}
	function drawyuan(x, y, r, color, isFill = true) {
		x *= iosScale;
		y *= iosScale;
		r *= iosScale;

		ctx.beginPath();
		ctx.globalAlpha = 1;
		ctx.lineWidth = 1;
		ctx.strokeStyle = color;
		ctx.fillStyle = color;
		ctx.arc(x, y, r, 0, 2 * Math.PI);
		ctx.stroke();
		ctx.closePath();
	}
	function getMin(arr) {
		var min = arr[0];
		for (var i = 1; i < arr.length; i++) {
			/* if(min>arr[i]){
				min=arr[i];
			} */
			min = min < arr[i] ? min : arr[i];
		}
		return min;
	}
	//取敌人屏幕距离
	function get2dDistance(x, y, x1, y1) {
		var osx = x - x1;
		var osy = y - y1;

		return Math.sqrt(osx * osx + osy * osy);
	}