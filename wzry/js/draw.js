		//绘制
		function clearCtx() {
			ctx.clearRect(0, 0, canvasDom.width, canvasDom.height);
		}

		function setLineWidth(w) {
			ctx.lineWidth = w;
		}

		function drawRectangle1(x, y, w, h, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			w *= iosScale;
			h *= iosScale;

			ctx.beginPath();
			ctx.fillStyle = 'rgba(255,255,255,0.7)';
			ctx.fillRect(x, y, w, h);
			ctx.closePath();
		}

		function drawRectangle(x, y, w, h, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			w *= iosScale;
			h *= iosScale;

			ctx.beginPath();

			ctx.fillStyle = color;
			ctx.globalAlpha = 0.5;
			ctx.fillRect(x, y, w, h,);

			ctx.closePath();
		}

		function drawLine(x1, y1, x2, y2, color) {
			x1 *= iosScale;
			y1 *= iosScale;
			x2 *= iosScale;
			y2 *= iosScale;

			ctx.beginPath();
			ctx.fillStyle = color;
			ctx.strokeStyle = color;
			ctx.globalAlpha = 1;
			ctx.moveTo(x1, y1);
			ctx.lineTo(x2, y2);
			ctx.stroke();
			ctx.closePath();
		}

		function drawCircle(x, y, r, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			r *= iosScale;

			ctx.beginPath();
			ctx.strokeStyle = color;
			ctx.fillStyle = color;
			ctx.globalAlpha = 1;
			ctx.arc(x, y, r, 0, 2 * Math.PI);
			if (isFill) {
				ctx.fill();
			} else {
				ctx.stroke();
			}
			ctx.closePath();
		}

		function drawRect(x, y, w, h, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			w *= iosScale;
			h *= iosScale;

			ctx.beginPath();
			ctx.strokeStyle = color;
			ctx.fillStyle = color;
			ctx.globalAlpha = 1;
			if (isFill) {
				ctx.fillRect(x, y, w, h);
			} else {
				ctx.strokeRect(x, y, w, h);
			}
			ctx.closePath();
		}


		function drawRoundRect(x, y, w, h, r, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			w *= iosScale;
			h *= iosScale;
			r *= iosScale;

			ctx.beginPath();
			ctx.strokeStyle = color;
			ctx.fillStyle = color;
			ctx.globalAlpha = 1;
			if (w < 2 * r) r = w / 2;
			if (h < 2 * r) r = h / 2;
			ctx.beginPath();
			ctx.moveTo(x + r, y);
			ctx.arcTo(x + w, y, x + w, y + h, r);
			ctx.arcTo(x + w, y + h, x, y + h, r);
			ctx.arcTo(x, y + h, x, y, r);
			ctx.arcTo(x, y, x + w, y, r);
			if (isFill) {
				ctx.fill();
			} else {
				ctx.stroke();
			}
			ctx.closePath();
		}

		function drawText1(text, x, y, size, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			size *= iosScale;

			ctx.beginPath();
			ctx.textAlign = "center";
			ctx.fillStyle = color;
			ctx.strokeStyle = color;
			ctx.font = size + 'px' + fontFamily;
			if (isFill) {
				ctx.fillText(text, x, y + size);
			} else {
				ctx.strokeText(text, x, y + size);
			}
			ctx.closePath();
		}


		function drawText(text, x, y, size, color, isFill = true) {
			x *= iosScale;
			y *= iosScale;
			size *= iosScale;

			ctx.beginPath();
			ctx.textAlign = "center";
			ctx.fillStyle = color;
			ctx.strokeStyle = color;
			ctx.globalAlpha = 1;
			ctx.font = size + 'px' + fontFamily;
			if (isFill) {
				ctx.fillText(text, x, y + size);
			} else {
				ctx.strokeText(text, x, y + size);
			}
			ctx.closePath();
		};