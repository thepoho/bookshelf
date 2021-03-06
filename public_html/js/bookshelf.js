Bookshelf = {
  websocket: null,
  lamp_states: ["lamp_off","lamp_on","lamp_flash_slow", "lamp_flash_fast"],
  startup: function(){
    Bookshelf.websocket = new WebSocket("ws://"+location.host+"/ws");
    Bookshelf.websocket.onopen = Bookshelf.socketOnOpen;
    Bookshelf.websocket.onmessage = Bookshelf.socketOnMessage;
    Bookshelf.websocket.onclose = function(ev) {console.log(ev);}

    $("table.bookshelf tr td").spectrum({
      change: Bookshelf.tdColourChanged,
      move: Bookshelf.tdColourChanged,
      showPalette: true,
      // palette: [ ],
      // showSelectionPalette: true, // true by default
      // selectionPalette: ["red", "green", "blue"]
    });

    $("a.test").click(function(){
      Bookshelf.setTestColours();
    });

    $("a.shutdown").click(function(){
      Bookshelf.sendMessage({message: "shutdown"});
    });

    $("a.test1").spectrum({
      change: Bookshelf.setAllColour,
      move: Bookshelf.setAllColour,
      showPalette: true,
    });

  },

  socketOnMessage: function(ev){
    console.log("WS response: "+ev.data);

    var data = $.parseJSON(ev.data);
    // if(data.name == "get_buttons"){
    //   Bookshelf.handleGetButtonsResponses(data.data);
    // }

  },

  socketOnOpen: function(ev){
    //let the server know we are ready
    Bookshelf.sendMessage({message: "ready"});
  },

  sendMessage: function(hash){
    var data = JSON.stringify(hash);
    console.log(data);
    Bookshelf.websocket.send(data);
  },

  log: function(str){
    var textarea = $("textarea.log");
    textarea.append(str+"\n");
    textarea[0].scrollTop = textarea[0].scrollHeight;
  },

  tdColourChanged: function(color){
    $(this).css("background-color", color.toHexString());
    Bookshelf.sendTdInfo($(this));
  },

  sendTdInfo: function(td){
    td = $(td);
    var color = new tinycolor(td.css("background-color"));
    var row = td.attr("data-row");
    var col = td.attr("data-col");
    var rgb = color.toRgb();
    var data = {
      message: "set_cell",
      r:   rgb.r,
      g:   rgb.g,
      b:   rgb.b,
      row: parseInt(row),
      col: parseInt(col)
    }
    Bookshelf.sendMessage(data);
  },

  setTestColours: function(){
    $.each($("table.bookshelf tr td"), function(idx, e){
      var num = 360 / ($("td").length) * idx;
      var str = "hsl("+num+", 100%, 50%)";
      $(e).css("background-color", str);
      Bookshelf.sendTdInfo(e);
    });
  },

  setAllColour: function(color){
    $.each($("table.bookshelf tr td"), function(idx, e){
      $(e).css("background-color", color.toHexString());
      Bookshelf.sendTdInfo(e);
    });
  }
}
