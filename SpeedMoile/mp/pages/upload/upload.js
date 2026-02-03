// pages/upload/upload.js
const app = getApp();

Page({
  data: {
    selectedFiles: []
  },

  // 选择图片
  chooseImages() {
    wx.chooseImage({
      count: 9, // 最多选择9张
      sizeType: ['original', 'compressed'],
      sourceType: ['album'], // 只从相册选择
      success: (res) => {
        const tempFilePaths = res.tempFilePaths;
        const selectedFiles = [...this.data.selectedFiles, ...tempFilePaths];
        
        this.setData({
          selectedFiles: selectedFiles.slice(0, 20) // 最多20张
        });
      }
    });
  },

  // 移除图片
  removeImage(e) {
    const { index } = e.currentTarget.dataset;
    const selectedFiles = [...this.data.selectedFiles];
    selectedFiles.splice(index, 1);
    
    this.setData({ selectedFiles });
  },

  // 上传图片
  uploadImages() {
    if (this.data.selectedFiles.length === 0) {
      wx.showToast({
        title: '请先选择图片',
        icon: 'none'
      });
      return;
    }

    wx.showLoading({ title: '上传中...' });

    let uploadedCount = 0;
    let failedCount = 0;
    const totalCount = this.data.selectedFiles.length;

    this.data.selectedFiles.forEach((filePath, index) => {
      wx.uploadFile({
        url: `${app.globalData.serverUrl}/api/upload`,
        filePath: filePath,
        name: 'image',
        success: (res) => {
          if (res.statusCode === 200) {
            uploadedCount++;
          } else {
            failedCount++;
          }
        },
        fail: () => {
          failedCount++;
        },
        complete: () => {
          if (uploadedCount + failedCount === totalCount) {
            wx.hideLoading();
            
            if (failedCount === 0) {
              wx.showToast({
                title: '全部上传成功',
                icon: 'success'
              });
              
              setTimeout(() => {
                wx.navigateBack();
              }, 1500);
            } else {
              wx.showModal({
                title: '上传完成',
                content: `成功 ${uploadedCount} 张，失败 ${failedCount} 张`,
                showCancel: false
              });
            }
            
            this.setData({ selectedFiles: [] });
          }
        }
      });
    });
  },

  // 预览图片
  previewImage(e) {
    const { index } = e.currentTarget.dataset;
    
    wx.previewImage({
      current: this.data.selectedFiles[index],
      urls: this.data.selectedFiles
    });
  }
})
