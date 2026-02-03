// pages/index/index.js
const app = getApp();

Page({
  data: {
    images: [],
    selectedImages: []
  },

  onLoad() {
    this.loadImages();
  },

  onShow() {
    this.loadImages();
  },

  // 加载图片列表
  loadImages() {
    wx.showLoading({ title: '加载中...' });
    
    wx.request({
      url: `${app.globalData.serverUrl}/api/images`,
      method: 'GET',
      success: (res) => {
        if (res.statusCode === 200) {
          this.setData({
            images: res.data.images || []
          });
        } else {
          wx.showToast({
            title: '加载失败',
            icon: 'error'
          });
        }
      },
      fail: () => {
        wx.showToast({
          title: '网络错误',
          icon: 'error'
        });
      },
      complete: () => {
        wx.hideLoading();
      }
    });
  },

  // 跳转到上传页面
  goToUpload() {
    wx.navigateTo({
      url: '/pages/upload/upload'
    });
  },

  // 预览图片
  previewImage(e) {
    const { url } = e.currentTarget.dataset;
    const urls = this.data.images.map(img => `${app.globalData.serverUrl}${img.url}`);
    
    wx.previewImage({
      current: `${app.globalData.serverUrl}${url}`,
      urls: urls
    });
  },

  // 下载图片
  downloadImage(e) {
    const { url, filename } = e.currentTarget.dataset;
    
    wx.showLoading({ title: '下载中...' });
    
    wx.downloadFile({
      url: `${app.globalData.serverUrl}${url}`,
      success: (res) => {
        if (res.statusCode === 200) {
          wx.saveImageToPhotosAlbum({
            filePath: res.tempFilePath,
            success: () => {
              wx.showToast({
                title: '保存成功',
                icon: 'success'
              });
            },
            fail: () => {
              wx.showToast({
                title: '保存失败',
                icon: 'error'
              });
            }
          });
        }
      },
      fail: () => {
        wx.showToast({
          title: '下载失败',
          icon: 'error'
        });
      },
      complete: () => {
        wx.hideLoading();
      }
    });
  },

  // 选择图片
  toggleSelect(e) {
    const { id } = e.currentTarget.dataset;
    const selectedImages = [...this.data.selectedImages];
    const index = selectedImages.indexOf(id);
    
    if (index > -1) {
      selectedImages.splice(index, 1);
    } else {
      selectedImages.push(id);
    }
    
    this.setData({ selectedImages });
  },

  // 批量删除
  deleteSelected() {
    if (this.data.selectedImages.length === 0) {
      wx.showToast({
        title: '请先选择图片',
        icon: 'none'
      });
      return;
    }

    wx.showModal({
      title: '确认删除',
      content: `确定要删除选中的 ${this.data.selectedImages.length} 张图片吗？`,
      success: (res) => {
        if (res.confirm) {
          this.performDelete();
        }
      }
    });
  },

  // 执行删除
  performDelete() {
    wx.showLoading({ title: '删除中...' });
    
    wx.request({
      url: `${app.globalData.serverUrl}/api/images/batch-delete`,
      method: 'POST',
      data: {
        ids: this.data.selectedImages
      },
      success: (res) => {
        if (res.statusCode === 200) {
          wx.showToast({
            title: '删除成功',
            icon: 'success'
          });
          this.setData({ selectedImages: [] });
          this.loadImages();
        } else {
          wx.showToast({
            title: '删除失败',
            icon: 'error'
          });
        }
      },
      fail: () => {
        wx.showToast({
          title: '网络错误',
          icon: 'error'
        });
      },
      complete: () => {
        wx.hideLoading();
      }
    });
  },

  // 刷新列表
  onPullDownRefresh() {
    this.loadImages();
    setTimeout(() => {
      wx.stopPullDownRefresh();
    }, 1000);
  }
})
