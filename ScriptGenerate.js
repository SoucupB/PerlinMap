function createElementFromHTML(htmlString) {
  var div = document.createElement('div');
  div.innerHTML = htmlString.trim();
  return div.firstChild;
}

function makeBuffer(list) {
  var stre = "<svg height='800' width='800'>";
  for(var i = 0; i < list.length; i += 6) {
    stre += "<polygon points = '";
    stre += list[i].toString() + ',' + list[i + 1].toString() + " ";
    stre += list[i + 2].toString() + ',' + list[i + 3].toString() + " ";
    stre += list[i + 4].toString() + ',' + list[i + 5].toString() + "' ";
    stre += 'style="fill:lime;stroke:purple;stroke-width:1" />'
  }
  return createElementFromHTML(stre + '</svg>');
}

function waitForElement(){
  if(isEverythingUp()){
    let perlin = new PerlinGenerator(800, 800, 3, 50);
    let triBuffer = perlin.getPerlinRandomMap();
    let bodyAdditionResponse = document.body.appendChild(makeBuffer(triBuffer));
    console.log(bodyAdditionResponse);
  }
  else{
    setTimeout(waitForElement, 250);
  }
}

waitForElement();
