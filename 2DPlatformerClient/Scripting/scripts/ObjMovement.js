function onObjMovement()
{
    var movement = new Vector2f(e.xVal, e.yVal);

    // move the object toward movement
    renderable.shape.setPosition(movement);

  //  // if is moving platform, set heading positive
  //  if (e.object.id.indexOf("MP") == 0) {
  //      e.object.headingPositive = e.positive;
  //  }

  //  // if is character set render offset
  //  if (e.object.id == "A") {
		//e.object.renderOffset = new Vector2f(e.offsetX, e.offsetY);
  //  }
}