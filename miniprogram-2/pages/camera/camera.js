// camera.js
Page({
  data: {
    devicePosition: 'back',
    flash: 'off',
    imageTempPath: '',
    isAnalyzing: false
  },

  onLoad() {
    // 检查相机权限
    this.checkCameraPermission()
  },

  // 检查相机权限
  checkCameraPermission() {
    wx.getSetting({
      success: (res) => {
        if (!res.authSetting['scope.camera']) {
          wx.authorize({
            scope: 'scope.camera',
            success: () => {
              console.log('相机授权成功')
            },
            fail: () => {
              wx.showModal({
                title: '需要相机权限',
                content: '请授权使用相机功能',
                showCancel: false
              })
            }
          })
        }
      }
    })
  },

  // 拍照
  takePhoto() {
    if (this.data.imageTempPath) {
      // 重拍
      this.setData({
        imageTempPath: ''
      })
      return
    }

    const ctx = wx.createCameraContext()
    ctx.takePhoto({
      quality: 'high',
      success: (res) => {
        this.setData({
          imageTempPath: res.tempImagePath
        })
      },
      fail: (err) => {
        console.error('拍照失败', err)
        wx.showToast({
          title: '拍照失败',
          icon: 'none'
        })
      }
    })
  },

  // 确认照片并生成故事
  confirmPhoto() {
    if (this.data.isAnalyzing) return

    this.setData({ isAnalyzing: true })
    wx.showLoading({
      title: '正在分析绘本...'
    })

    // 模拟AI分析过程
    setTimeout(() => {
      this.generateStory()
    }, 2000)
  },

  // 生成故事
  generateStory() {
    const imageTempPath = this.data.imageTempPath
    const now = new Date()

    // 模拟AI生成的故事内容
    const mockStories = [
      {
        title: '勇敢的小兔子',
        content: '从前有一只可爱的小兔子,它住在一个美丽的森林里。有一天,小兔子决定出去探险。它穿过了小溪,翻过了小山,遇到了许多有趣的朋友。最后,小兔子带着满满的收获回到了家,它明白了一个道理:只要勇敢尝试,就能发现这个世界的美好。',
        tags: ['勇敢', '冒险', '友谊']
      },
      {
        title: '星星的愿望',
        content: '天上的星星有一颗特别的小星星,它有一个大大的愿望。它想看看地面上的世界。于是,它努力地向下坠落,变成了一颗流星。它飞过了森林,飞过了海洋,最后落在了一个小女孩的手心里。小女孩把它放到了罐子里,每天和它说话。从此,小星星再也不孤单了。',
        tags: ['愿望', '友情', '梦想']
      },
      {
        title: '彩虹桥',
        content: '雨过天晴,天空中出现了一道美丽的彩虹桥。小动物们都跑出来看,它们想爬到桥的另一边去看看。小鸟飞得最快,最先到达了彩虹桥。小兔子也跳了上去,小熊慢慢悠悠地爬着。它们在彩虹桥上看到了世界上最美的风景,成为了最好的朋友。',
        tags: ['友谊', '美好', '合作']
      }
    ]

    const randomStory = mockStories[Math.floor(Math.random() * mockStories.length)]

    const newStory = {
      id: Date.now(),
      image: imageTempPath,
      title: randomStory.title,
      content: randomStory.content,
      tags: randomStory.tags,
      date: `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(2, '0')}-${String(now.getDate()).padStart(2, '0')}`,
      time: `${String(now.getHours()).padStart(2, '0')}:${String(now.getMinutes()).padStart(2, '0')}`
    }

    // 保存到本地存储
    try {
      let stories = wx.getStorageSync('storyList') || []
      stories.unshift(newStory)
      wx.setStorageSync('storyList', stories)

      wx.hideLoading()
      wx.showToast({
        title: '故事生成成功',
        icon: 'success'
      })

      setTimeout(() => {
        wx.redirectTo({
          url: `/pages/story/story?id=${newStory.id}`
        })
      }, 1500)
    } catch (e) {
      console.error('保存失败', e)
      wx.hideLoading()
      wx.showToast({
        title: '保存失败',
        icon: 'none'
      })
    }
  },

  // 切换前后摄像头
  switchCamera() {
    this.setData({
      devicePosition: this.data.devicePosition === 'back' ? 'front' : 'back'
    })
  },

  // 切换闪光灯
  switchFlash() {
    this.setData({
      flash: this.data.flash === 'off' ? 'on' : 'off'
    })
  },

  // 相机错误处理
  cameraError(e) {
    console.error('相机错误', e)
    wx.showModal({
      title: '相机错误',
      content: '无法打开相机,请检查权限设置',
      showCancel: false
    })
  }
})
