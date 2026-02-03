const express = require('express');
const multer = require('multer');
const cors = require('cors');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 3000;

// 中间件
app.use(cors());
app.use(express.json());
app.use('/uploads', express.static(path.join(__dirname, 'uploads')));

// 确保上传目录存在
const uploadDir = path.join(__dirname, 'uploads');
if (!fs.existsSync(uploadDir)) {
  fs.mkdirSync(uploadDir, { recursive: true });
}

// 配置 multer 文件上传
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, uploadDir);
  },
  filename: (req, file, cb) => {
    // 生成唯一文件名
    const uniqueSuffix = Date.now() + '-' + Math.round(Math.random() * 1E9);
    const ext = path.extname(file.originalname);
    cb(null, `screenshot-${uniqueSuffix}${ext}`);
  }
});

const upload = multer({
  storage: storage,
  limits: {
    fileSize: 10 * 1024 * 1024 // 限制10MB
  },
  fileFilter: (req, file, cb) => {
    // 只允许图片文件
    const allowedTypes = /jpeg|jpg|png|gif|webp/;
    const extname = allowedTypes.test(path.extname(file.originalname).toLowerCase());
    const mimetype = allowedTypes.test(file.mimetype);
    
    if (mimetype && extname) {
      return cb(null, true);
    } else {
      cb(new Error('只支持图片文件！'));
    }
  }
});

// 数据存储（简单实现，生产环境应使用数据库）
const dataFile = path.join(__dirname, 'data.json');

// 读取数据
function readData() {
  try {
    if (fs.existsSync(dataFile)) {
      const data = fs.readFileSync(dataFile, 'utf8');
      return JSON.parse(data);
    }
  } catch (error) {
    console.error('读取数据失败:', error);
  }
  return { images: [] };
}

// 写入数据
function writeData(data) {
  try {
    fs.writeFileSync(dataFile, JSON.stringify(data, null, 2));
    return true;
  } catch (error) {
    console.error('写入数据失败:', error);
    return false;
  }
}

// API 路由

// 1. 上传图片
app.post('/api/upload', upload.single('image'), (req, res) => {
  try {
    if (!req.file) {
      return res.status(400).json({ error: '未选择文件' });
    }

    const data = readData();
    const imageInfo = {
      id: Date.now().toString() + Math.random().toString(36).substr(2, 9),
      filename: req.file.originalname,
      savedFilename: req.file.filename,
      url: `/uploads/${req.file.filename}`,
      size: req.file.size,
      uploadDate: new Date().toISOString(),
      uploadTime: Date.now()
    };

    data.images.push(imageInfo);
    
    if (writeData(data)) {
      res.json({
        success: true,
        message: '上传成功',
        image: imageInfo
      });
    } else {
      res.status(500).json({ error: '保存数据失败' });
    }
  } catch (error) {
    console.error('上传错误:', error);
    res.status(500).json({ error: '上传失败' });
  }
});

// 2. 获取图片列表
app.get('/api/images', (req, res) => {
  try {
    const data = readData();
    
    // 按上传时间倒序排列
    const sortedImages = data.images.sort((a, b) => b.uploadTime - a.uploadTime);
    
    res.json({
      success: true,
      images: sortedImages.map(img => ({
        id: img.id,
        filename: img.filename,
        url: img.url,
        size: img.size,
        uploadDate: new Date(img.uploadDate).toLocaleDateString('zh-CN')
      }))
    });
  } catch (error) {
    console.error('获取列表错误:', error);
    res.status(500).json({ error: '获取列表失败' });
  }
});

// 3. 删除单张图片
app.delete('/api/images/:id', (req, res) => {
  try {
    const { id } = req.params;
    const data = readData();
    
    const imageIndex = data.images.findIndex(img => img.id === id);
    
    if (imageIndex === -1) {
      return res.status(404).json({ error: '图片不存在' });
    }
    
    const image = data.images[imageIndex];
    const filePath = path.join(__dirname, 'uploads', image.savedFilename);
    
    // 删除文件
    if (fs.existsSync(filePath)) {
      fs.unlinkSync(filePath);
    }
    
    // 从数据中移除
    data.images.splice(imageIndex, 1);
    
    if (writeData(data)) {
      res.json({
        success: true,
        message: '删除成功'
      });
    } else {
      res.status(500).json({ error: '保存数据失败' });
    }
  } catch (error) {
    console.error('删除错误:', error);
    res.status(500).json({ error: '删除失败' });
  }
});

// 4. 批量删除图片
app.post('/api/images/batch-delete', (req, res) => {
  try {
    const { ids } = req.body;
    
    if (!Array.isArray(ids) || ids.length === 0) {
      return res.status(400).json({ error: '无效的ID列表' });
    }
    
    const data = readData();
    let deletedCount = 0;
    
    ids.forEach(id => {
      const imageIndex = data.images.findIndex(img => img.id === id);
      
      if (imageIndex !== -1) {
        const image = data.images[imageIndex];
        const filePath = path.join(__dirname, 'uploads', image.savedFilename);
        
        // 删除文件
        if (fs.existsSync(filePath)) {
          fs.unlinkSync(filePath);
        }
        
        // 从数据中移除
        data.images.splice(imageIndex, 1);
        deletedCount++;
      }
    });
    
    if (writeData(data)) {
      res.json({
        success: true,
        message: `成功删除 ${deletedCount} 张图片`,
        deletedCount
      });
    } else {
      res.status(500).json({ error: '保存数据失败' });
    }
  } catch (error) {
    console.error('批量删除错误:', error);
    res.status(500).json({ error: '批量删除失败' });
  }
});

// 5. 健康检查
app.get('/api/health', (req, res) => {
  res.json({
    success: true,
    message: 'API服务正常运行',
    timestamp: new Date().toISOString()
  });
});

// 错误处理中间件
app.use((error, req, res, next) => {
  console.error('服务器错误:', error);
  res.status(500).json({
    error: error.message || '服务器内部错误'
  });
});

// 启动服务器
app.listen(PORT, () => {
  console.log(`截图整理API服务器运行在 http://localhost:${PORT}`);
  console.log(`上传目录: ${uploadDir}`);
});
