namespace ImageSmall;

partial class Form1
{
    /// <summary>
    ///  Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    ///  Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
        if (disposing && (components != null))
        {
            components.Dispose();
        }
        base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    ///  Required method for Designer support - do not modify
    ///  the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
        this.btnSelectFiles = new System.Windows.Forms.Button();
        this.btnCompress = new System.Windows.Forms.Button();
        this.lblQuality = new System.Windows.Forms.Label();
        this.trackBarQuality = new System.Windows.Forms.TrackBar();
        this.lblQualityValue = new System.Windows.Forms.Label();
        this.listBoxFiles = new System.Windows.Forms.ListBox();
        this.progressBar = new System.Windows.Forms.ProgressBar();
        this.lblStatus = new System.Windows.Forms.Label();
        this.btnSelectOutput = new System.Windows.Forms.Button();
        this.txtOutputFolder = new System.Windows.Forms.TextBox();
        this.lblOutputFolder = new System.Windows.Forms.Label();
        this.btnClearList = new System.Windows.Forms.Button();
        this.btnSelectFolder = new System.Windows.Forms.Button();
        ((System.ComponentModel.ISupportInitialize)(this.trackBarQuality)).BeginInit();
        this.SuspendLayout();
        // 
        // btnSelectFiles
        // 
        this.btnSelectFiles.Location = new System.Drawing.Point(12, 12);
        this.btnSelectFiles.Name = "btnSelectFiles";
        this.btnSelectFiles.Size = new System.Drawing.Size(120, 30);
        this.btnSelectFiles.TabIndex = 0;
        this.btnSelectFiles.Text = "选择图片";
        this.btnSelectFiles.UseVisualStyleBackColor = true;
        this.btnSelectFiles.Click += new System.EventHandler(this.btnSelectFiles_Click);
        // 
        // btnSelectFolder
        // 
        this.btnSelectFolder.Location = new System.Drawing.Point(264, 12);
        this.btnSelectFolder.Name = "btnSelectFolder";
        this.btnSelectFolder.Size = new System.Drawing.Size(120, 30);
        this.btnSelectFolder.TabIndex = 2;
        this.btnSelectFolder.Text = "选择文件夹";
        this.btnSelectFolder.UseVisualStyleBackColor = true;
        this.btnSelectFolder.Click += new System.EventHandler(this.btnSelectFolder_Click);
        // 
        // btnCompress
        // 
        this.btnCompress.Location = new System.Drawing.Point(12, 410);
        this.btnCompress.Name = "btnCompress";
        this.btnCompress.Size = new System.Drawing.Size(120, 30);
        this.btnCompress.TabIndex = 11;
        this.btnCompress.Text = "开始压缩";
        this.btnCompress.UseVisualStyleBackColor = true;
        this.btnCompress.Click += new System.EventHandler(this.btnCompress_Click);
        // 
        // lblQuality
        // 
        this.lblQuality.AutoSize = true;
        this.lblQuality.Location = new System.Drawing.Point(12, 330);
        this.lblQuality.Name = "lblQuality";
        this.lblQuality.Size = new System.Drawing.Size(68, 17);
        this.lblQuality.TabIndex = 7;
        this.lblQuality.Text = "压缩质量:";
        // 
        // trackBarQuality
        // 
        this.trackBarQuality.Location = new System.Drawing.Point(100, 325);
        this.trackBarQuality.Maximum = 100;
        this.trackBarQuality.Minimum = 10;
        this.trackBarQuality.Name = "trackBarQuality";
        this.trackBarQuality.Size = new System.Drawing.Size(500, 45);
        this.trackBarQuality.TabIndex = 8;
        this.trackBarQuality.Value = 75;
        this.trackBarQuality.Scroll += new System.EventHandler(this.trackBarQuality_Scroll);
        // 
        // lblQualityValue
        // 
        this.lblQualityValue.AutoSize = true;
        this.lblQualityValue.Location = new System.Drawing.Point(620, 330);
        this.lblQualityValue.Name = "lblQualityValue";
        this.lblQualityValue.Size = new System.Drawing.Size(32, 17);
        this.lblQualityValue.TabIndex = 9;
        this.lblQualityValue.Text = "75%";
        // 
        // listBoxFiles
        // 
        this.listBoxFiles.FormattingEnabled = true;
        this.listBoxFiles.ItemHeight = 17;
        this.listBoxFiles.Location = new System.Drawing.Point(12, 48);
        this.listBoxFiles.Name = "listBoxFiles";
        this.listBoxFiles.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
        this.listBoxFiles.Size = new System.Drawing.Size(760, 174);
        this.listBoxFiles.TabIndex = 3;
        // 
        // progressBar
        // 
        this.progressBar.Location = new System.Drawing.Point(12, 376);
        this.progressBar.Name = "progressBar";
        this.progressBar.Size = new System.Drawing.Size(760, 23);
        this.progressBar.TabIndex = 10;
        // 
        // lblStatus
        // 
        this.lblStatus.AutoSize = true;
        this.lblStatus.Location = new System.Drawing.Point(150, 418);
        this.lblStatus.Name = "lblStatus";
        this.lblStatus.Size = new System.Drawing.Size(44, 17);
        this.lblStatus.TabIndex = 12;
        this.lblStatus.Text = "就绪";
        // 
        // btnSelectOutput
        // 
        this.btnSelectOutput.Location = new System.Drawing.Point(652, 230);
        this.btnSelectOutput.Name = "btnSelectOutput";
        this.btnSelectOutput.Size = new System.Drawing.Size(120, 30);
        this.btnSelectOutput.TabIndex = 6;
        this.btnSelectOutput.Text = "浏览...";
        this.btnSelectOutput.UseVisualStyleBackColor = true;
        this.btnSelectOutput.Click += new System.EventHandler(this.btnSelectOutput_Click);
        // 
        // txtOutputFolder
        // 
        this.txtOutputFolder.Location = new System.Drawing.Point(100, 233);
        this.txtOutputFolder.Name = "txtOutputFolder";
        this.txtOutputFolder.Size = new System.Drawing.Size(546, 25);
        this.txtOutputFolder.TabIndex = 5;
        // 
        // lblOutputFolder
        // 
        this.lblOutputFolder.AutoSize = true;
        this.lblOutputFolder.Location = new System.Drawing.Point(12, 236);
        this.lblOutputFolder.Name = "lblOutputFolder";
        this.lblOutputFolder.Size = new System.Drawing.Size(68, 17);
        this.lblOutputFolder.TabIndex = 4;
        this.lblOutputFolder.Text = "输出目录:";
        // 
        // btnClearList
        // 
        this.btnClearList.Location = new System.Drawing.Point(138, 12);
        this.btnClearList.Name = "btnClearList";
        this.btnClearList.Size = new System.Drawing.Size(120, 30);
        this.btnClearList.TabIndex = 1;
        this.btnClearList.Text = "清空列表";
        this.btnClearList.UseVisualStyleBackColor = true;
        this.btnClearList.Click += new System.EventHandler(this.btnClearList_Click);
        // 
        // Form1
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.ClientSize = new System.Drawing.Size(784, 461);
        this.Controls.Add(this.btnSelectFolder);
        this.Controls.Add(this.btnClearList);
        this.Controls.Add(this.lblOutputFolder);
        this.Controls.Add(this.txtOutputFolder);
        this.Controls.Add(this.btnSelectOutput);
        this.Controls.Add(this.lblStatus);
        this.Controls.Add(this.progressBar);
        this.Controls.Add(this.listBoxFiles);
        this.Controls.Add(this.lblQualityValue);
        this.Controls.Add(this.trackBarQuality);
        this.Controls.Add(this.lblQuality);
        this.Controls.Add(this.btnCompress);
        this.Controls.Add(this.btnSelectFiles);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;
        this.Name = "Form1";
        this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
        this.Text = "图片压缩工具 - ImageSmall";
        ((System.ComponentModel.ISupportInitialize)(this.trackBarQuality)).EndInit();
        this.ResumeLayout(false);
        this.PerformLayout();
    }

    #endregion

    private System.Windows.Forms.Button btnSelectFiles;
    private System.Windows.Forms.Button btnCompress;
    private System.Windows.Forms.Label lblQuality;
    private System.Windows.Forms.TrackBar trackBarQuality;
    private System.Windows.Forms.Label lblQualityValue;
    private System.Windows.Forms.ListBox listBoxFiles;
    private System.Windows.Forms.ProgressBar progressBar;
    private System.Windows.Forms.Label lblStatus;
    private System.Windows.Forms.Button btnSelectOutput;
    private System.Windows.Forms.TextBox txtOutputFolder;
    private System.Windows.Forms.Label lblOutputFolder;
    private System.Windows.Forms.Button btnClearList;
    private System.Windows.Forms.Button btnSelectFolder;
}
