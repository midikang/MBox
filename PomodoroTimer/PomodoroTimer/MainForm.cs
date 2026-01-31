namespace PomodoroTimer
{
    public partial class MainForm : Form
    {
        private enum TimerMode
        {
            Work,
            ShortBreak,
            LongBreak
        }

        private TimerMode currentMode = TimerMode.Work;
        private int timeRemaining; // in seconds
        private int totalTime; // in seconds
        private bool isRunning = false;
        private int completedSessions = 0;

        // Timer durations in seconds
        private const int WORK_TIME = 25 * 60; // 25 minutes
        private const int SHORT_BREAK_TIME = 5 * 60; // 5 minutes
        private const int LONG_BREAK_TIME = 15 * 60; // 15 minutes

        public MainForm()
        {
            InitializeComponent();
            SetTimerMode(TimerMode.Work);
        }

        private void SetTimerMode(TimerMode mode)
        {
            currentMode = mode;
            
            switch (mode)
            {
                case TimerMode.Work:
                    totalTime = WORK_TIME;
                    lblStatus.Text = "工作时间";
                    lblStatus.ForeColor = Color.FromArgb(220, 38, 38); // Red
                    break;
                case TimerMode.ShortBreak:
                    totalTime = SHORT_BREAK_TIME;
                    lblStatus.Text = "短休息";
                    lblStatus.ForeColor = Color.FromArgb(34, 197, 94); // Green
                    break;
                case TimerMode.LongBreak:
                    totalTime = LONG_BREAK_TIME;
                    lblStatus.Text = "长休息";
                    lblStatus.ForeColor = Color.FromArgb(59, 130, 246); // Blue
                    break;
            }
            
            timeRemaining = totalTime;
            UpdateTimerDisplay();
            UpdateProgressBar();
        }

        private void UpdateTimerDisplay()
        {
            int minutes = timeRemaining / 60;
            int seconds = timeRemaining % 60;
            lblTimer.Text = $"{minutes:D2}:{seconds:D2}";
            
            // Update window title
            this.Text = $"{lblTimer.Text} - 番茄钟";
        }

        private void UpdateProgressBar()
        {
            if (totalTime > 0)
            {
                int progress = (int)(((totalTime - timeRemaining) / (double)totalTime) * 100);
                progressBar.Value = Math.Min(progress, 100);
            }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            isRunning = true;
            timer.Start();
            btnStart.Enabled = false;
            btnPause.Enabled = true;
            
            // Disable mode selection buttons while running
            btnWorkTimer.Enabled = false;
            btnShortBreak.Enabled = false;
            btnLongBreak.Enabled = false;
        }

        private void btnPause_Click(object sender, EventArgs e)
        {
            isRunning = false;
            timer.Stop();
            btnStart.Enabled = true;
            btnPause.Enabled = false;
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            isRunning = false;
            timer.Stop();
            SetTimerMode(currentMode);
            
            btnStart.Enabled = true;
            btnPause.Enabled = false;
            
            // Re-enable mode selection buttons
            btnWorkTimer.Enabled = true;
            btnShortBreak.Enabled = true;
            btnLongBreak.Enabled = true;
        }

        private void btnWorkTimer_Click(object sender, EventArgs e)
        {
            SetTimerMode(TimerMode.Work);
        }

        private void btnShortBreak_Click(object sender, EventArgs e)
        {
            SetTimerMode(TimerMode.ShortBreak);
        }

        private void btnLongBreak_Click(object sender, EventArgs e)
        {
            SetTimerMode(TimerMode.LongBreak);
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            if (isRunning && timeRemaining > 0)
            {
                timeRemaining--;
                UpdateTimerDisplay();
                UpdateProgressBar();

                if (timeRemaining == 0)
                {
                    OnTimerComplete();
                }
            }
        }

        private void OnTimerComplete()
        {
            timer.Stop();
            isRunning = false;
            
            // Play system beep
            System.Media.SystemSounds.Beep.Play();
            
            // Update session count if it was a work session
            if (currentMode == TimerMode.Work)
            {
                completedSessions++;
                lblSessionCount.Text = $"完成的番茄钟: {completedSessions}";
            }
            
            // Show completion message
            string message = currentMode switch
            {
                TimerMode.Work => "工作时间结束！该休息一下了。",
                TimerMode.ShortBreak => "短休息结束！准备继续工作。",
                TimerMode.LongBreak => "长休息结束！准备继续工作。",
                _ => "时间到！"
            };
            
            MessageBox.Show(message, "番茄钟提醒", MessageBoxButtons.OK, MessageBoxIcon.Information);
            
            // Reset buttons
            btnStart.Enabled = true;
            btnPause.Enabled = false;
            btnWorkTimer.Enabled = true;
            btnShortBreak.Enabled = true;
            btnLongBreak.Enabled = true;
            
            // Auto-suggest next mode
            if (currentMode == TimerMode.Work)
            {
                // After work, suggest a break
                if (completedSessions % 4 == 0)
                {
                    SetTimerMode(TimerMode.LongBreak);
                }
                else
                {
                    SetTimerMode(TimerMode.ShortBreak);
                }
            }
            else
            {
                // After break, suggest work
                SetTimerMode(TimerMode.Work);
            }
        }
    }
}
