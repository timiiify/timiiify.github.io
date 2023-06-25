//内存
function readShort(addr) {
	return Number(h5gg.getValue(addr, "I16"));
}
function readInt(addr) {
	return Number(h5gg.getValue(addr, "I32"));
}

function readLong(addr) {
	return Number(h5gg.getValue(addr, "I64"));
}

function readFloat(addr) {
	return Number(h5gg.getValue(addr, "F32"));
}

function isNull(addr) {
	return (addr < 0x100000000 || addr > 0x300000000);
}
function writeInt(addr, value) {
	return (h5gg.setValue(addr, value, "I32"));
}
function writeFloat(addr, value) {
	return (h5gg.setValue(addr, value, "F32"));
}

					// var matrix = [];
			// var juzhen = readLong(readLong(baseAddr + 0x8523008) + 0x20);
			// for (var i = 0; i < 16; i++) {
			// 	MatrixAddr = readFloat(juzhen + 0x270 + i * 0x4);
			// 	matrix.push(MatrixAddr);
			// }
							//相机
									// var camear = matrix[3] * worldPos.X + matrix[7] * worldPos.Y + matrix[11] * worldPos.Z + matrix[15];
									// if (camear >= 0.01) {
									// 	//屏幕XY
									// 	var playerdis = parseInt(camear / 100);
									// 	var screenX = sWidth / 2 + (matrix[0] * worldPos.X + matrix[4] * worldPos.Y + matrix[8] * worldPos.Z + matrix[12]) / camear * sWidth / 2;//X
									// 	var screenY = sHeight / 2 - (matrix[1] * worldPos.X + matrix[5] * worldPos.Y + matrix[9] * (worldPos.Z + 100) + matrix[13]) / camear * sHeight / 2;//Y
									// 	var screenY2 = sHeight / 2 - (matrix[1] * worldPos.X + matrix[5] * worldPos.Y + matrix[9] * (worldPos.Z - 100) + matrix[13]) / camear * sHeight / 2;//Y
									// }
									// var boxheight = screenY2 - screenY;
									// var boxwidth = boxheight * 0.5;
									// var boxleft = screenX - boxwidth * 0.5;
									// var boxtop = screenY;