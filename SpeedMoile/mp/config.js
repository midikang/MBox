// config.js - 配置文件
const config = {
  // 开发环境配置
  development: {
    serverUrl: 'http://localhost:3000'
  },
  
  // 生产环境配置
  production: {
    serverUrl: 'https://your-domain.com' // 替换为实际的生产环境域名
  }
};

// 根据环境变量选择配置，默认使用开发环境
const env = 'development'; // 生产环境改为 'production'

module.exports = config[env];
