# Joy's Coffee Shop - 网站部署指南 / Deployment Guide

欢迎使用 Joy's Coffee Shop 网站！本指南将帮助您将网站发布到互联网上。

Welcome to Joy's Coffee Shop website! This guide will help you publish your website to the internet.

## 📋 目录 / Table of Contents

1. [快速开始 / Quick Start](#快速开始--quick-start)
2. [部署选项 / Deployment Options](#部署选项--deployment-options)
3. [推荐部署方案 / Recommended Deployment](#推荐部署方案--recommended-deployment)
4. [域名配置 / Domain Configuration](#域名配置--domain-configuration)
5. [SEO 优化建议 / SEO Optimization Tips](#seo-优化建议--seo-optimization-tips)

---

## 🚀 快速开始 / Quick Start

### 本地预览 / Local Preview

在部署之前，您可以在本地预览网站：

Before deploying, you can preview the website locally:

```bash
# 进入 website 目录 / Navigate to website directory
cd website

# 使用简单的 HTTP 服务器 / Use a simple HTTP server
# 选项 1: 使用 Python / Option 1: Using Python
python -m http.server 8000

# 选项 2: 使用 Node.js (需要先安装 http-server) / Option 2: Using Node.js
npx http-server -p 8000

# 选项 3: 使用 PHP / Option 3: Using PHP
php -S localhost:8000
```

然后在浏览器中访问 / Then visit in your browser:
```
http://localhost:8000
```

---

## 🌐 部署选项 / Deployment Options

### 1. GitHub Pages (免费 / Free) ⭐ 推荐 / Recommended

**优点 / Advantages:**
- ✅ 完全免费 / Completely free
- ✅ 自动 HTTPS / Automatic HTTPS
- ✅ 快速部署 / Fast deployment
- ✅ 支持自定义域名 / Custom domain support
- ✅ 全球 CDN / Global CDN

**部署步骤 / Deployment Steps:**

1. **将代码推送到 GitHub / Push code to GitHub**
   ```bash
   # 如果还没有创建仓库 / If you haven't created a repository yet
   git init
   git add .
   git commit -m "Initial commit: Joy's Coffee Shop website"
   git branch -M main
   git remote add origin https://github.com/YOUR_USERNAME/joys-coffee-shop.git
   git push -u origin main
   ```

2. **启用 GitHub Pages / Enable GitHub Pages**
   - 访问仓库设置 / Go to repository Settings
   - 点击 "Pages" 选项 / Click "Pages" option
   - 在 "Source" 下选择 "main" 分支和 "/website" 文件夹 / Under "Source", select "main" branch and "/website" folder
   - 点击 "Save" / Click "Save"
   - 等待几分钟，您的网站将发布在 / Wait a few minutes, your site will be published at:
     ```
     https://YOUR_USERNAME.github.io/joys-coffee-shop/
     ```

### 2. Vercel (免费 / Free) ⭐ 推荐 / Recommended

**优点 / Advantages:**
- ✅ 免费托管静态网站 / Free static site hosting
- ✅ 极快的全球 CDN / Very fast global CDN
- ✅ 自动 HTTPS / Automatic HTTPS
- ✅ 每次推送自动部署 / Auto-deploy on every push

**部署步骤 / Deployment Steps:**

1. 访问 [vercel.com](https://vercel.com) 并注册 / Visit vercel.com and sign up
2. 点击 "New Project" / Click "New Project"
3. 导入您的 GitHub 仓库 / Import your GitHub repository
4. 设置项目 / Configure project:
   - Framework Preset: 选择 "Other" / Select "Other"
   - Root Directory: 设置为 `website` / Set to `website`
5. 点击 "Deploy" / Click "Deploy"
6. 完成！您的网站将获得一个 vercel.app 域名 / Done! Your site will get a vercel.app domain

### 3. Netlify (免费 / Free)

**优点 / Advantages:**
- ✅ 免费托管 / Free hosting
- ✅ 持续部署 / Continuous deployment
- ✅ 表单处理 / Form handling
- ✅ 自动 HTTPS / Automatic HTTPS

**部署步骤 / Deployment Steps:**

1. **方法 1: 拖拽部署 / Method 1: Drag & Drop**
   - 访问 [netlify.com](https://www.netlify.com)
   - 注册并登录 / Sign up and login
   - 将 `website` 文件夹拖到部署区域 / Drag the `website` folder to the deploy area
   - 完成！/ Done!

2. **方法 2: Git 集成 / Method 2: Git Integration**
   - 在 Netlify 中点击 "New site from Git" / Click "New site from Git" in Netlify
   - 连接您的 GitHub 仓库 / Connect your GitHub repository
   - Build settings:
     - Base directory: `website`
     - Build command: (留空 / leave empty)
     - Publish directory: `website`
   - 点击 "Deploy site" / Click "Deploy site"

### 4. 阿里云 OSS / Aliyun OSS (适合中国用户 / For Chinese users)

**优点 / Advantages:**
- ✅ 在中国访问速度快 / Fast access in China
- ✅ 支持自定义域名 / Custom domain support
- ✅ CDN 加速 / CDN acceleration

**部署步骤 / Deployment Steps:**

1. 创建 OSS Bucket / Create OSS Bucket
2. 设置读写权限为 "公共读" / Set permissions to "Public Read"
3. 上传 website 文件夹中的所有文件 / Upload all files from website folder
4. 启用静态网站托管 / Enable static website hosting
5. 设置默认首页为 `index.html` / Set default homepage to `index.html`
6. (可选) 绑定自定义域名和配置 CDN / (Optional) Bind custom domain and configure CDN

### 5. 腾讯云 COS / Tencent Cloud COS (适合中国用户 / For Chinese users)

类似阿里云 OSS 的步骤 / Similar steps to Aliyun OSS

---

## 🌍 域名配置 / Domain Configuration

### 购买域名 / Purchase Domain

**推荐域名注册商 / Recommended Domain Registrars:**
- [阿里云万网](https://wanwang.aliyun.com/) (中国 / China)
- [腾讯云](https://cloud.tencent.com/) (中国 / China)
- [Namecheap](https://www.namecheap.com/) (国际 / International)
- [GoDaddy](https://www.godaddy.com/) (国际 / International)

**域名建议 / Domain Suggestions:**
- `joys-coffee.com`
- `joys-coffee.cn`
- `joyscoffeeshop.com`
- `joy-nanjing.com`

### 配置自定义域名 / Configure Custom Domain

#### GitHub Pages:
1. 在仓库设置中找到 "Pages" / Go to "Pages" in repository settings
2. 在 "Custom domain" 中输入您的域名 / Enter your domain in "Custom domain"
3. 在域名提供商处添加 DNS 记录 / Add DNS records at your domain provider:
   ```
   Type: A
   Name: @
   Value: 185.199.108.153, 185.199.109.153, 185.199.110.153, 185.199.111.153
   
   Type: CNAME
   Name: www
   Value: YOUR_USERNAME.github.io
   ```

#### Vercel/Netlify:
1. 在项目设置中点击 "Domains" / Click "Domains" in project settings
2. 添加您的域名 / Add your domain
3. 按照提示配置 DNS 记录 / Configure DNS records as instructed

---

## 📈 SEO 优化建议 / SEO Optimization Tips

### 已实现的 SEO 功能 / Implemented SEO Features

✅ **语义化 HTML / Semantic HTML**
- 正确使用 header, nav, section, footer 等标签
- Proper use of header, nav, section, footer tags

✅ **Meta 标签优化 / Meta Tags Optimization**
- Title, Description, Keywords
- Open Graph tags (社交媒体分享 / Social media sharing)
- Twitter Card tags
- Structured Data (JSON-LD)

✅ **性能优化 / Performance Optimization**
- Preconnect for Google Fonts
- Optimized CSS and JavaScript

✅ **移动端适配 / Mobile Responsive**
- Viewport meta tag
- Responsive design

### 进一步优化建议 / Further Optimization Suggestions

1. **添加图片 / Add Images**
   - 使用高质量的咖啡店照片 / Use high-quality coffee shop photos
   - 优化图片大小 (使用 WebP 格式) / Optimize image size (use WebP format)
   - 添加 alt 属性 / Add alt attributes

2. **Google Search Console**
   ```bash
   # 添加网站到 Google Search Console / Add site to Google Search Console
   # 提交 sitemap / Submit sitemap
   https://yourwebsite.com/sitemap.xml
   ```

3. **创建 sitemap.xml / Create sitemap.xml**
   ```xml
   <?xml version="1.0" encoding="UTF-8"?>
   <urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
     <url>
       <loc>https://yourwebsite.com/</loc>
       <lastmod>2024-01-01</lastmod>
       <priority>1.0</priority>
     </url>
   </urlset>
   ```

4. **创建 robots.txt / Create robots.txt**
   ```
   User-agent: *
   Allow: /
   Sitemap: https://yourwebsite.com/sitemap.xml
   ```

5. **添加 Google Analytics**
   - 注册 Google Analytics / Sign up for Google Analytics
   - 在 `</head>` 标签前添加跟踪代码 / Add tracking code before `</head>` tag

6. **社交媒体集成 / Social Media Integration**
   - 创建微信公众号 / Create WeChat Official Account
   - 创建微博账号 / Create Weibo account
   - 创建小红书账号 / Create Xiaohongshu account
   - 更新网站中的社交媒体链接 / Update social media links in website

---

## 📱 微信小程序部署 / WeChat Mini Program (可选 / Optional)

如果您想创建微信小程序版本 / If you want to create a WeChat Mini Program version:

1. 注册微信小程序账号 / Register WeChat Mini Program account
2. 使用 Taro 或 uni-app 框架转换网站 / Use Taro or uni-app framework to convert website
3. 提交审核 / Submit for review

---

## 🔧 维护和更新 / Maintenance and Updates

### 更新网站内容 / Update Website Content

```bash
# 1. 编辑文件 / Edit files
# 2. 提交更改 / Commit changes
git add .
git commit -m "Update: 描述更改内容"
git push

# 如果使用 Vercel/Netlify/GitHub Pages，网站会自动更新
# If using Vercel/Netlify/GitHub Pages, the site will auto-update
```

### 监控网站 / Monitor Website

- 使用 Google Analytics 跟踪访问量 / Use Google Analytics to track visits
- 使用 Google Search Console 监控 SEO / Use Google Search Console to monitor SEO
- 定期检查网站加载速度 / Regularly check website loading speed
  - [PageSpeed Insights](https://pagespeed.web.dev/)
  - [GTmetrix](https://gtmetrix.com/)

---

## 📞 技术支持 / Technical Support

如有问题，请查看以下资源 / For questions, check these resources:

- **GitHub Pages 文档** / GitHub Pages Docs: https://docs.github.com/pages
- **Vercel 文档** / Vercel Docs: https://vercel.com/docs
- **Netlify 文档** / Netlify Docs: https://docs.netlify.com

---

## ✨ 下一步 / Next Steps

1. ✅ **本地测试** / Test locally
2. ✅ **选择部署平台** / Choose deployment platform
3. ✅ **部署网站** / Deploy website
4. ✅ **配置自定义域名** (可选) / Configure custom domain (optional)
5. ✅ **添加 Google Analytics** / Add Google Analytics
6. ✅ **提交到 Google Search Console** / Submit to Google Search Console
7. ✅ **分享到社交媒体** / Share on social media

---

## 🎉 恭喜！/ Congratulations!

您的 Joy's Coffee Shop 网站已经准备好发布了！

Your Joy's Coffee Shop website is ready to be published!

祝您的咖啡店生意兴隆！☕🧡

Wishing your coffee shop great success! ☕🧡

---

**Made with ❤️ for Joy's Coffee Shop**
