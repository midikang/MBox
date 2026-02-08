// index.js
Page({
  data: {
    storyList: []
  },

  onLoad() {
    this.loadStoryList()
  },

  onShow() {
    this.loadStoryList()
  },

  // 加载绘本故事列表
  loadStoryList() {
    try {
      const stories = wx.getStorageSync('storyList') || []
      this.setData({
        storyList: stories
      })
    } catch (e) {
      console.error('加载绘本列表失败', e)
    }
  },

  // 拍照按钮点击
  takePhoto() {
    wx.navigateTo({
      url: '/pages/camera/camera'
    })
  },

  // 查看绘本故事
  viewStory(e) {
    const { id } = e.currentTarget.dataset
    wx.navigateTo({
      url: `/pages/story/story?id=${id}`
    })
  }
})
