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
    })

    // $("select.game_state").change(function(){
    //   var data = {
    //     message: "set_game_state",
    //     value: $(this).val()
    //   };
    //   Bookshelf.sendMessage(data);
    // });

    //prevent long presses from opening context menu
    window.oncontextmenu = function(event) {
      event.preventDefault();
      event.stopPropagation();
      return false;
    };

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
    Bookshelf.websocket.send(data);
  },

  log: function(str){
    var textarea = $("textarea.log");
    textarea.append(str+"\n");
    textarea[0].scrollTop = textarea[0].scrollHeight;
  },

  tdColourChanged: function(color){
    $(this).css("background-color", color.toHexString());
    window.poho = color;
    Bookshelf.sendTdInfo($(this));
  },

  sendTdInfo: function(td){
    td = $(td);
    var color = new tinycolor(td.css("background-color"));
    var row = td.attr("data-row");
    var col = td.attr("data-col");
    var data = {
      message: "set_cell",
      value: color.toRgb(),
      row: row,
      col: col
    }
    Bookshelf.sendMessage(data);
  },

  setTestColours: function(){
    $.each($("table.bookshelf tr td"), function(idx, e){
      var num = 360 / 25 * idx;
      var str = "hsl("+num+", 100%, 50%)";
      $(e).css("background-color", str);
      Bookshelf.sendTdInfo(e);
    });
  }
}
