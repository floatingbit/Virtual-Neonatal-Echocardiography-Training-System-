namespace CalculationsPackage
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            OpenWire.Proxy.SourcePin sourcePin3 = new OpenWire.Proxy.SourcePin();
            Mitov.VideoLab.DSAudioPreview dsAudioPreview1 = new Mitov.VideoLab.DSAudioPreview();
            OpenWire.Proxy.SinkPin sinkPin2 = new OpenWire.Proxy.SinkPin();
            OpenWire.Proxy.StatePin statePin1 = new OpenWire.Proxy.StatePin();
            OpenWire.Proxy.SinkPin sinkPin3 = new OpenWire.Proxy.SinkPin();
            Mitov.AudioLab.DSGraph dsGraph2 = new Mitov.AudioLab.DSGraph();
            OpenWire.Proxy.SourcePin sourcePin4 = new OpenWire.Proxy.SourcePin();
            OpenWire.Proxy.SourcePin sourcePin5 = new OpenWire.Proxy.SourcePin();
            OpenWire.Proxy.StatePin statePin2 = new OpenWire.Proxy.StatePin();
            OpenWire.Proxy.StatePin statePin3 = new OpenWire.Proxy.StatePin();
            OpenWire.Proxy.SourcePin sourcePin6 = new OpenWire.Proxy.SourcePin();
            Mitov.VideoLab.DSVideoPreview dsVideoPreview1 = new Mitov.VideoLab.DSVideoPreview();
            Mitov.VideoLab.AspectRatio aspectRatio1 = new Mitov.VideoLab.AspectRatio();
            Mitov.AudioLab.DSGraph dsGraph1 = new Mitov.AudioLab.DSGraph();
            OpenWire.Proxy.SourcePin sourcePin1 = new OpenWire.Proxy.SourcePin();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            OpenWire.Proxy.SinkPin sinkPin1 = new OpenWire.Proxy.SinkPin();
            OpenWire.Proxy.SourcePin sourcePin2 = new OpenWire.Proxy.SourcePin();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.lbInstructions = new System.Windows.Forms.Label();
            this.dsVideoPlayer1 = new Mitov.VideoLab.DSVideoPlayer(this.components);
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.lbF4 = new System.Windows.Forms.Label();
            this.lbF3 = new System.Windows.Forms.Label();
            this.lbF1 = new System.Windows.Forms.Label();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lbF2 = new System.Windows.Forms.Label();
            this.tableLayoutPanel8 = new System.Windows.Forms.TableLayoutPanel();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.dsImageDisplay1 = new Mitov.VideoLab.DSImageDisplay(this.components);
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dsVideoPlayer1)).BeginInit();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dsImageDisplay1)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Controls.Add(this.lbInstructions, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel1, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 87.66234F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 12.33766F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(745, 545);
            this.tableLayoutPanel2.TabIndex = 1;
            // 
            // lbInstructions
            // 
            this.lbInstructions.AutoSize = true;
            this.lbInstructions.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.lbInstructions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbInstructions.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbInstructions.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.lbInstructions.Location = new System.Drawing.Point(0, 477);
            this.lbInstructions.Margin = new System.Windows.Forms.Padding(0);
            this.lbInstructions.Name = "lbInstructions";
            this.lbInstructions.Size = new System.Drawing.Size(745, 68);
            this.lbInstructions.TabIndex = 4;
            this.lbInstructions.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // dsVideoPlayer1
            // 
            sourcePin3.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin3.ConnectionData")));
            this.dsVideoPlayer1.AudioOutputPin = sourcePin3;
            this.dsVideoPlayer1.AudioPreview = dsAudioPreview1;
            sinkPin2.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sinkPin2.ConnectionData")));
            this.dsVideoPlayer1.ClockPin = sinkPin2;
            statePin1.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("statePin1.ConnectionData")));
            this.dsVideoPlayer1.EnablePin = statePin1;
            this.dsVideoPlayer1.FileName = "C:\\Virtual Echocardiographic Training System\\v1.0\\Videos\\79.avi";
            sinkPin3.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sinkPin3.ConnectionData")));
            this.dsVideoPlayer1.FileNamePin = sinkPin3;
            sourcePin4.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin4.ConnectionData")));
            dsGraph2.GraphPin = sourcePin4;
            this.dsVideoPlayer1.Graph = dsGraph2;
            this.dsVideoPlayer1.Loop = true;
            sourcePin5.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin5.ConnectionData")));
            this.dsVideoPlayer1.OutputPin = sourcePin5;
            statePin2.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("statePin2.ConnectionData")));
            this.dsVideoPlayer1.PausePin = statePin2;
            statePin3.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("statePin3.ConnectionData")));
            this.dsVideoPlayer1.ProgressPin = statePin3;
            sourcePin6.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin6.ConnectionData")));
            this.dsVideoPlayer1.TextOutputPin = sourcePin6;
            dsVideoPreview1.InternalData = ((Vcl.VclBinaryData)(resources.GetObject("dsVideoPreview1.InternalData")));
            dsVideoPreview1.PreviewDock = null;
            this.dsVideoPlayer1.VideoPreview = dsVideoPreview1;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.lbF4, 0, 3);
            this.tableLayoutPanel4.Controls.Add(this.lbF3, 0, 2);
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 4;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(200, 100);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // lbF4
            // 
            this.lbF4.AutoSize = true;
            this.lbF4.BackColor = System.Drawing.Color.White;
            this.lbF4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbF4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbF4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbF4.Location = new System.Drawing.Point(0, 60);
            this.lbF4.Margin = new System.Windows.Forms.Padding(0);
            this.lbF4.Name = "lbF4";
            this.lbF4.Size = new System.Drawing.Size(200, 40);
            this.lbF4.TabIndex = 30;
            this.lbF4.Text = "F4   DOPPLER";
            this.lbF4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbF3
            // 
            this.lbF3.AutoSize = true;
            this.lbF3.BackColor = System.Drawing.Color.White;
            this.lbF3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbF3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbF3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbF3.Location = new System.Drawing.Point(0, 40);
            this.lbF3.Margin = new System.Windows.Forms.Padding(0);
            this.lbF3.Name = "lbF3";
            this.lbF3.Size = new System.Drawing.Size(200, 20);
            this.lbF3.TabIndex = 29;
            this.lbF3.Text = "F3   M-MODE";
            this.lbF3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbF1
            // 
            this.lbF1.AutoSize = true;
            this.lbF1.BackColor = System.Drawing.Color.White;
            this.lbF1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbF1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbF1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbF1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.lbF1.Location = new System.Drawing.Point(0, 0);
            this.lbF1.Margin = new System.Windows.Forms.Padding(0);
            this.lbF1.Name = "lbF1";
            this.lbF1.Size = new System.Drawing.Size(96, 51);
            this.lbF1.TabIndex = 27;
            this.lbF1.Text = "F1   2D";
            this.lbF1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 3;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 75F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel5.Controls.Add(this.tableLayoutPanel6, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.tableLayoutPanel7, 2, 0);
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(200, 100);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 1;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Controls.Add(this.comboBox2, 0, 0);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel6.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 2;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 8.620689F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 91.37931F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(30, 100);
            this.tableLayoutPanel6.TabIndex = 0;
            // 
            // comboBox2
            // 
            this.comboBox2.CausesValidation = false;
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.Enabled = false;
            this.comboBox2.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(3, 3);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(24, 21);
            this.comboBox2.TabIndex = 1;
            this.comboBox2.TabStop = false;
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel7.ColumnCount = 1;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel7.Controls.Add(this.label1, 0, 3);
            this.tableLayoutPanel7.Controls.Add(this.label2, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.label3, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.lbF2, 0, 1);
            this.tableLayoutPanel7.Location = new System.Drawing.Point(180, 0);
            this.tableLayoutPanel7.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 4;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(20, 100);
            this.tableLayoutPanel7.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.White;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(0, 75);
            this.label1.Margin = new System.Windows.Forms.Padding(0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(20, 25);
            this.label1.TabIndex = 30;
            this.label1.Text = "F4   DOPPLER";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.White;
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(0, 50);
            this.label2.Margin = new System.Windows.Forms.Padding(0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(20, 25);
            this.label2.TabIndex = 29;
            this.label2.Text = "F3   M-MODE";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.White;
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label3.Location = new System.Drawing.Point(0, 0);
            this.label3.Margin = new System.Windows.Forms.Padding(0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(20, 25);
            this.label3.TabIndex = 27;
            this.label3.Text = "F1   2D";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbF2
            // 
            this.lbF2.AutoSize = true;
            this.lbF2.BackColor = System.Drawing.Color.White;
            this.lbF2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbF2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbF2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbF2.Location = new System.Drawing.Point(0, 25);
            this.lbF2.Margin = new System.Windows.Forms.Padding(0);
            this.lbF2.Name = "lbF2";
            this.lbF2.Size = new System.Drawing.Size(20, 25);
            this.lbF2.TabIndex = 28;
            this.lbF2.Text = "F2   2D + COLOR";
            this.lbF2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // tableLayoutPanel8
            // 
            this.tableLayoutPanel8.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel8.ColumnCount = 1;
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel8.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel8.Name = "tableLayoutPanel8";
            this.tableLayoutPanel8.RowCount = 1;
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel8.Size = new System.Drawing.Size(200, 100);
            this.tableLayoutPanel8.TabIndex = 0;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(3, 44);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(105, 430);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.pictureBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.dsImageDisplay1);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(114, 3);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(552, 471);
            this.panel3.TabIndex = 3;
            // 
            // dsImageDisplay1
            // 
            aspectRatio1.Height = ((uint)(3u));
            aspectRatio1.Mode = Mitov.VideoLab.AspectRatioMode.None;
            aspectRatio1.Width = ((uint)(4u));
            this.dsImageDisplay1.AspectRatio = aspectRatio1;
            this.dsImageDisplay1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.dsImageDisplay1.Center = false;
            this.dsImageDisplay1.Dock = System.Windows.Forms.DockStyle.Fill;
            sourcePin1.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin1.ConnectionData")));
            dsGraph1.GraphPin = sourcePin1;
            this.dsImageDisplay1.Graph = dsGraph1;
            sinkPin1.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sinkPin1.ConnectionData")));
            this.dsImageDisplay1.InputPin = sinkPin1;
            this.dsImageDisplay1.InternalData = ((Vcl.VclBinaryData)(resources.GetObject("dsImageDisplay1.InternalData")));
            this.dsImageDisplay1.Location = new System.Drawing.Point(0, 0);
            this.dsImageDisplay1.Margin = new System.Windows.Forms.Padding(0);
            this.dsImageDisplay1.Name = "dsImageDisplay1";
            this.dsImageDisplay1.Size = new System.Drawing.Size(552, 471);
            this.dsImageDisplay1.Stretch = true;
            this.dsImageDisplay1.TabIndex = 1;
            this.dsImageDisplay1.Text = "dsImageDisplay1";
            sourcePin2.ConnectionData = ((OpenWire.PinConnections)(resources.GetObject("sourcePin2.ConnectionData")));
            this.dsImageDisplay1.UserControlOutputPin = sourcePin2;
            this.dsImageDisplay1.Visible = true;
            this.dsImageDisplay1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 75F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel1.Controls.Add(this.panel3, 1, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(745, 477);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ClientSize = new System.Drawing.Size(745, 545);
            this.Controls.Add(this.tableLayoutPanel2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "MainForm";
            this.Text = "Calculations";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.FormLoad);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dsVideoPlayer1)).EndInit();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dsImageDisplay1)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private Mitov.VideoLab.DSVideoPlayer dsVideoPlayer1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Label lbF4;
        private System.Windows.Forms.Label lbF3;
        private System.Windows.Forms.Label lbF1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lbF2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel8;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label lbInstructions;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel3;
        private Mitov.VideoLab.DSImageDisplay dsImageDisplay1;

    }
}

