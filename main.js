
const electron = require('electron');
// Module to control application life.
const { app } = electron;
var fs=require('fs');
// Module to create native browser window.
const { BrowserWindow } = electron;
const { ipcMain } = require('electron');
const dialog = require('electron').dialog
// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let win;
var test = require("./build/Debug/test");
var newestimg="src/test.jpeg";
var history=new Array(newestimg);
var future=new Array();

ipcMain.on('hide-window', () => {
  console.log("11");
  win.minimize();
});
//最大化 
ipcMain.on('show-window', () => {
  win.maximize();
});
//还原
ipcMain.on('orignal-window', () => {
  win.unmaximize();
});

ipcMain.on('back', (event) => {
  console.log(newestimg);
  
  var temp=history.pop();
  console.log(temp);

  if(temp==newestimg){
    temp=history.pop();
  }
  newestimg=temp;
  future.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
  console.log("back");
  console.log(newestimg);
});
ipcMain.on('open-file-dialog', function (event) {
  dialog.showOpenDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
      event.sender.send('selected-directory', files);
      newestimg=files;
      history=new Array(newestimg);
    }
  })
})
ipcMain.on('save-file-dialog', function (event) {
  dialog.showSaveDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
      fs.writeFileSync(files, fs.readFileSync(newestimg));
      event.sender.send('saved', files); 
    }
  })
})
ipcMain.on('window-all-closed', () => {
  // On OS X it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit();
  }
});
ipcMain.on('2Gray',(event, message) => {
  var data= test.Convert(newestimg,'Gray');
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('2Green',(event, message) => {
  var data= test.Convert(newestimg,'Green');
  newestimg=data;
  console.log(data);
  history.push(newestimg);
  

  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('2Blue',(event, message) => {
  var data= test.Convert(newestimg,'Blue');
  newestimg=data;
  console.log(data);
  history.push(newestimg);

  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('2Red',(event, message) => {
  console.log("Red");
  var data= test.Convert(newestimg,'Red');
  newestimg=data;
  console.log(data);
  history.push(newestimg);

  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('hue',(event, message) => {
  var data= test.Convert(newestimg,'hue',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('autocontrast',(event) => {
  console.log('autocontrast'+newestimg);
  var data= test.Convert(newestimg,'autocontrast');
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('saturation',(event, message) => {
  console.log('saturation'+message+newestimg);

  var data= test.Convert(newestimg,'saturation',message);
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('lightness',(event, message) => {
  console.log('lightness'+message+newestimg);
  var data= test.Convert(newestimg,'lightness',message);
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('contrast',(event, message) => {
  console.log('contrast'+message+newestimg);

  var data= test.Convert(newestimg,'contrast',message);
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('gauss',(event, message1,message2,message3,message4) => {
  console.log('MAINgauss'+message1+newestimg+message2+message3+message4);
  var data= test.Convert(newestimg,'gauss',message1,message2,message3,message4);
  newestimg=data;
  console.log(newestimg+"\n");
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
  console.log(newestimg+"\n");
});
ipcMain.on('median',(event, message1,message2,message3) => {
  var data= test.Convert(newestimg,'median',message1,message2,message3);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('mean',(event, message1,message2,message3) => {
  var data= test.Convert(newestimg,'mean',message1,message2,message3);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});

ipcMain.on('add',(event) => {
  var data=newestimg;
  dialog.showOpenDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
      data= test.Convert(newestimg,'add',files);    
      newestimg=data;
      console.log(newestimg);
      history.push(newestimg);
      event.sender.send("UpdateImage", newestimg);
    }
  })
  
  
});

ipcMain.on('substract',(event) => {
  var data;
  dialog.showOpenDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
      var data= test.Convert(newestimg,'substract',files);
      newestimg=data;
      console.log(newestimg);
      history.push(newestimg);
      event.sender.send("UpdateImage", newestimg);
    }
  })
  
});
ipcMain.on('mutiply',(event) => {
  var data=newestimg;
  dialog.showOpenDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
       data= test.Convert(newestimg,'mutiply',files);
      newestimg=data;
      console.log(newestimg);
      history.push(newestimg);
      event.sender.send("UpdateImage", newestimg);
    }
  })
 
});
ipcMain.on('divide',(event) => {
  var newone=newestimg;
  dialog.showOpenDialog({
    properties: ['openFile'], 
    filters: [ { name: 'Images', extensions: ['jpg', 'png'] }]
    }, function (files) {
    if (files) {
      var data= test.Convert(newestimg,'divide',files);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
    }
  });
  
});

ipcMain.on('scale',(event, message) => {
  var data= test.Convert(newestimg,'scale',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('rotate',(event, message) => {
  var data= test.Convert(newestimg,'rotate',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('cut',(event, message1,message2,message3,message4) => {
  var data= test.Convert(newestimg,'cut', message1,message2,message3,message4);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('distancetransform',(event) => {
  var data= test.Convert(newestimg,'distancetransform');
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('skeleton',(event) => {
  var data= test.Convert(newestimg,'skeleton');
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('skeletonrebuild',(event) => {
  var data= test.Convert(newestimg,'skeletonrebuild');
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});

ipcMain.on('dilate',(event, message) => {
  var data= test.Convert(newestimg,'dilate',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('erode',(event, message) => {
  var data= test.Convert(newestimg,'erode',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('openning',(event, message) => {
  var data= test.Convert(newestimg,'openning',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('closing',(event, message) => {
  var data= test.Convert(newestimg,'closing',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('Mrebuild',(event, message) => {
  var data= test.Convert(newestimg,'Mrebuild',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('thick',(event, message1,message2) => {
  console.log("mainn");
  console.log(message2);
  var data= test.Convert(newestimg,'thick', message1,message2);
  newestimg=data;
 
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('thin',(event,  message1,message2) => {
  var data= test.Convert(newestimg,'thin', message1,message2);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});ipcMain.on('Gdilate',(event, message) => {
  var data= test.Convert(newestimg,'Gdilate',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('Gerode',(event, message) => {
  var data= test.Convert(newestimg,'Gerode',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('Gopenning',(event, message) => {
  var data= test.Convert(newestimg,'Gopenning',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('Gclosing',(event, message) => {
  var data= test.Convert(newestimg,'Gclosing',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('GMrebuild',(event, message) => {
  var data= test.Convert(newestimg,'Gdilate',message);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('Binaryostu',(event) => {
  console.log(newestimg);
  var data= test.Convert(newestimg,'Binaryostu');
  newestimg=data;
 
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('Binary',(event,message1,message2) => {
  var data= test.Convert(newestimg,'Binary', message1,message2);
  newestimg=data;
  console.log(newestimg);
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('detectsobel',(event) => {
  console.log(detect_sobel);
  var data= test.Convert(newestimg,'detectsobel');
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('detectlaplace',(event) => {
  console.log(detect_laplace);
  var data= test.Convert(newestimg,'detectlaplace');
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('detectcanny',(event) => {
  console.log(detect_canny);
  var data= test.Convert(newestimg,'detectcanny');
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
ipcMain.on('watershed',(event) => {
  console.log(watershed);
  var data= test.Convert(newestimg,'watershed');
  newestimg=data;
  history.push(newestimg);
  event.sender.send("UpdateImage", newestimg);
});
function createWindow() {
  // Create the browser window.
  win = new BrowserWindow({ width: 800, height: 600, minWidth: 600, minHeight: 400, titleBarStyle: "hidden", frame: false });

  win.openDevTools();
  // and load the index.html of the app.
  win.loadURL(`file://${__dirname}/index.html`);


  app.addRecentDocument('/Users/USERNAME/Desktop/work.type');
  win.on('closed', () => {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    win = null;
  });


}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow);


// Quit when all windows are closed.
app.on('window-all-closed', () => {
  // On OS X it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (win === null) {
    createWindow();
  }
});

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.