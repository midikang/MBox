// app.js
App({
  onLaunch() {
    // 初始化存储
    if (!wx.getStorageSync('storyList')) {
      wx.setStorageSync('storyList', [])
    }
  },
  globalData: {
    userInfo: null
  }
})
