// story.js
Page({
  data: {
    storyId: '',
    story: {},
    isReading: false,
    innerAudioContext: null
  },

  onLoad(options) {
    if (options.id) {
      this.setData({ storyId: options.id })
      this.loadStory(options.id)
    } else {
      wx.showToast({
        title: '故事不存在',
        icon: 'none'
      })
      setTimeout(() => {
        wx.navigateBack()
      }, 1500)
    }
  },

  onUnload() {
    // 页面卸载时停止朗读并释放资源
    this.stopRead()
  },

  // 加载绘本故事
  loadStory(id) {
    try {
      const stories = wx.getStorageSync('storyList') || []
      const story = stories.find(item => item.id === parseInt(id))
      if (story) {
        this.setData({ story })
      } else {
        wx.showToast({
          title: '故事不存在',
          icon: 'none'
        })
        setTimeout(() => {
          wx.navigateBack()
        }, 1500)
      }
    } catch (e) {
      console.error('加载故事失败', e)
      wx.showToast({
        title: '加载失败',
        icon: 'none'
      })
    }
  },

  // 开始朗读
  toggleRead() {
    if (!this.data.story.content) {
      wx.showToast({
        title: '故事内容为空',
        icon: 'none'
      })
      return
    }

    // 停止之前的朗读
    wx.createInnerAudioContext && this.stopRead()

    this.setData({
      isReading: true
    })

    // 使用模拟播放(最稳定方案)
    this.playVoice(this.data.story.content)
  },

  // 播放语音(使用简单的进度模拟方案)
  playVoice(text) {
    // 计算朗读时长:根据文本长度估算,每个字约200ms
    const duration = Math.max(5000, text.length * 200)

    wx.showToast({
      title: '正在朗读故事...',
      icon: 'none',
      duration: duration
    })

    // 使用定时器模拟朗读过程
    this.readingTimer = setTimeout(() => {
      this.setData({ isReading: false })
      wx.showToast({
        title: '朗读完成',
        icon: 'success',
        duration: 2000
      })
    }, duration)

    // 显示当前阅读内容(逐字高亮效果)
    this.showReadingAnimation(text, duration)
  },

  // 显示阅读动画
  showReadingAnimation(text, totalDuration) {
    const sentences = this.splitSentences(text)
    let currentIndex = 0
    const intervalTime = totalDuration / sentences.length

    this.readingInterval = setInterval(() => {
      if (!this.data.isReading || currentIndex >= sentences.length) {
        clearInterval(this.readingInterval)
        return
      }

      // 可以在这里添加逐句显示的动画效果
      currentIndex++
    }, intervalTime)
  },

  // 清理音频资源
  cleanupAudio(filePath) {
    if (this.data.innerAudioContext) {
      try {
        this.data.innerAudioContext.stop()
        this.data.innerAudioContext.destroy()
      } catch (e) {}
      this.setData({ innerAudioContext: null })
    }

    if (filePath) {
      try {
        const fs = wx.getFileSystemManager()
        fs.unlinkSync(filePath)
      } catch (e) {
        console.log('删除临时文件失败', e)
      }
    }
  },

  // 分句处理
  splitSentences(text) {
    // 按照标点符号分句
    const sentences = []
    const patterns = /[。！？.!?;；]/g
    let lastIndex = 0
    let match

    while ((match = patterns.exec(text)) !== null) {
      const sentence = text.substring(lastIndex, match.index + 1).trim()
      if (sentence) {
        sentences.push(sentence)
      }
      lastIndex = match.index + 1
    }

    // 添加剩余部分
    const remaining = text.substring(lastIndex).trim()
    if (remaining) {
      sentences.push(remaining)
    }

    return sentences.length > 0 ? sentences : [text]
  },

  // 停止朗读
  stopRead() {
    this.setData({ isReading: false })

    // 清理定时器
    if (this.readingTimer) {
      clearTimeout(this.readingTimer)
      this.readingTimer = null
    }

    if (this.readingInterval) {
      clearInterval(this.readingInterval)
      this.readingInterval = null
    }

    // 清理音频资源
    if (this.data.innerAudioContext) {
      try {
        this.data.innerAudioContext.stop()
        this.data.innerAudioContext.destroy()
      } catch (e) {}
      this.setData({ innerAudioContext: null })
    }

    wx.hideLoading()
    wx.hideToast()
  },

  // 分享故事
  shareStory() {
    wx.showShareMenu({
      withShareTicket: true
    })
  },

  // 分享到好友
  onShareAppMessage() {
    return {
      title: this.data.story.title || '快来听我分享的绘本故事',
      path: `/pages/story/story?id=${this.data.storyId}`,
      imageUrl: this.data.story.image
    }
  },

  // 删除故事
  deleteStory() {
    wx.showModal({
      title: '确认删除',
      content: '确定要删除这个绘本故事吗？',
      success: (res) => {
        if (res.confirm) {
          try {
            let stories = wx.getStorageSync('storyList') || []
            stories = stories.filter(item => item.id !== parseInt(this.data.storyId))
            wx.setStorageSync('storyList', stories)

            wx.showToast({
              title: '删除成功',
              icon: 'success'
            })

            setTimeout(() => {
              wx.navigateBack()
            }, 1500)
          } catch (e) {
            console.error('删除失败', e)
            wx.showToast({
              title: '删除失败',
              icon: 'none'
            })
          }
        }
      }
    })
  }
})
