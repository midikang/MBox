// app.js
const config = require('./config.js');

App({
  globalData: {
    serverUrl: config.serverUrl
  },
  
  onLaunch() {
    console.log('截图整理助手启动');
    console.log('服务器地址:', this.globalData.serverUrl);
  }
})
