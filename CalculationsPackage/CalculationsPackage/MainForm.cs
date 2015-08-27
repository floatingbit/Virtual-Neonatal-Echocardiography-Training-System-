using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using FrameGrabber;
using System.Drawing.Imaging;
using CalculationsPackage.Reports;

namespace CalculationsPackage
{
    public partial class MainForm : Form
    {
        public CalculationWindow calculationsDialogue;
        private string[] instructionSetLV;
        private string[] instructionSetLA_AO;
        private string[] instructionSet2D;
        private string[] instructionSetAortic;
        private string[] instructionSetPulmonary;
        private string[] instructionSetSVC;
        private string[] instructionSetPDA;
        private string[] instructionSetRVPressure;
        private int mouse_key_input_counter;
        private Calculations calculationPackage;
        private FullReport fullReport;
        public static CalculationModes calculationMode;
        public bool is2DMode;
        public Point lastPointClicked;
        public Point startLocation;
        public List<Point> pointsClicked;
        private Bitmap imgFrame;
        private bool offline_video_4_measurements;

        //Patient Info
        public static double weight;

        private double percentPosition;
        public static List<int> areaInPixels;
        public static List<double> lengthsInPixels;

        private Image lastScreenShot;

        //New variable added
        public string currentVideoPath;

        public MainForm(string currentVideoPath)
        {
            this.currentVideoPath = currentVideoPath;

            InitializeComponent();
            
            this.mouse_key_input_counter = 0;

            this.calculationsDialogue = new CalculationWindow();

            this.instructionSetLV = new string[20];
            this.instructionSetLA_AO = new string[20];
            this.instructionSet2D = new string[20];
            this.instructionSetAortic = new string[20];
            this.instructionSetPulmonary = new string[20];
            this.instructionSetSVC = new string[20];
            this.instructionSetPDA = new string[6];
            this.instructionSetRVPressure = new string[6];

            this.CreateInstructionSet();

            this.calculationPackage = new Calculations();

            this.fullReport = new FullReport();

            this.lastPointClicked = new Point();
            this.startLocation = new Point();

            this.pointsClicked = new List<Point>();
            MainForm.areaInPixels = new List<int>();
            MainForm.lengthsInPixels = new List<double>();

            this.offline_video_4_measurements = false;

            this.ShowDialog();
        }

        public MainForm()
        {
            //this.currentVideoPath = currentVideoPath;

            InitializeComponent();

            this.mouse_key_input_counter = 0;

            this.calculationsDialogue = new CalculationWindow();

            this.instructionSetLV = new string[20];
            this.instructionSetLA_AO = new string[20];
            this.instructionSet2D = new string[20];
            this.instructionSetAortic = new string[20];
            this.instructionSetPulmonary = new string[20];
            this.instructionSetSVC = new string[20];
            this.instructionSetPDA = new string[6];
            this.instructionSetRVPressure = new string[6];

            this.CreateInstructionSet();

            this.calculationPackage = new Calculations();

            this.fullReport = new FullReport();

            this.lastPointClicked = new Point();
            this.startLocation = new Point();

            this.pointsClicked = new List<Point>();
            MainForm.areaInPixels = new List<int>();
            MainForm.lengthsInPixels = new List<double>();

            this.offline_video_4_measurements = false;

            this.ShowDialog();
        }
                
        private void CreateInstructionSet()
        {
            //Instructions for LV M-Mode
            this.instructionSetLV[0] = "Press Enter to start Depth and time calibration..";

            this.instructionSetLV[1] = "Click on initial cm marker";
            this.instructionSetLV[2] = "Click on the 4th subsequent cm marker";
            this.instructionSetLV[3] = "Click on initial second marker";
            this.instructionSetLV[4] = "Click on subsequent second marker";

            this.instructionSetLV[5] = "Press Enter to start M-Mode Markers..";

            this.instructionSetLV[6] = "Click on endocardium of anterior RV wall in diastole";
            this.instructionSetLV[7] = "Click on endocardium of anterior RV septum in diastole";
            this.instructionSetLV[8] = "Click on endocardium of anterior LV septum in diastole";
            this.instructionSetLV[9] = "Click on endocardium of LV posterior wall in diastole";
            this.instructionSetLV[10] = "Click on pericardium of LV posterior wall in diastole";
            this.instructionSetLV[11] = "Click on endocardium of RV septum in systole";
            this.instructionSetLV[12] = "Click on endocardium of LV septum in systole";
            this.instructionSetLV[13] = "Click on endocardium of LV posterior wall in systole";
            this.instructionSetLV[14] = "Click on pericadium of LV posterior wall in systole";
            this.instructionSetLV[15] = "Click on intial EKG";
            this.instructionSetLV[16] = "Click on following EKG";


            //Instructions for LA_AO
            this.instructionSetLA_AO[0] = "Press Enter to start Depth calibration..";

            this.instructionSetLA_AO[1] = "Click on initial cm marker";
            this.instructionSetLA_AO[2] = "Click on the 5th subsequent cm marker";

            this.instructionSetLA_AO[3] = "Press Enter to start M-Mode Markers..";

            this.instructionSetLA_AO[4] = "Click on aorta anterior wall in systole";
            this.instructionSetLA_AO[5] = "Click on aorta posterior wall in systole";
            this.instructionSetLA_AO[6] = "Click on LA posterior wall endocardium in diastole";

            //Instructions for 2D
            this.instructionSet2D[0] = "Click on initial cm marker";
            this.instructionSet2D[1] = "Click on the 3rd subsequent cm marker";
            this.instructionSet2D[2] = "Select appropriate frame in end diastole and hit Enter";
            this.instructionSet2D[3] = "Trace endocardium of LV and hit Enter to close the region";
            this.instructionSet2D[4] = "Click on the center of the base";
            this.instructionSet2D[5] = "Click on the apex of the endocardium";
            this.instructionSet2D[6] = "Trace epicardium of LV and hit Enter to close the region";
            this.instructionSet2D[7] = "Click on the center of the base";
            this.instructionSet2D[8] = "Click on the apex of the endocardium";
            this.instructionSet2D[9] = "Select the next frame and hit Enter";
            this.instructionSet2D[10] = "Trace endocardium of LV and hit Enter to close the region";
            this.instructionSet2D[11] = "Click on the center of the base";
            this.instructionSet2D[12] = "Click on the apex of the endocardium";
            this.instructionSet2D[13] = "Trace epicardium of LV and hit Enter to close the region";
            this.instructionSet2D[14] = "Click on the center of the base";
            this.instructionSet2D[15] = "Click on the apex of the endocardium";
            this.instructionSet2D[16] = "Press enter to create the report";

            //Instructions for Aortic
            this.instructionSetAortic[0] = "Select the appropriate frame and hit Enter";
            this.instructionSetAortic[1] = "Click on initial velocity marker";
            this.instructionSetAortic[2] = "Click on second subsequent velocity marker";
            this.instructionSetAortic[3] = "Click on initial time marker";
            this.instructionSetAortic[4] = "Click on second subsequent time marker";
            this.instructionSetAortic[5] = "Click on start of systole";
            this.instructionSetAortic[6] = "Click on subsequent start of systole";
            this.instructionSetAortic[7] = "Trace aortic flow velocity contour and hit Enter to close the region";
            this.instructionSetAortic[8] = "Hit Enter to continue..";
            this.instructionSetAortic[10] = "Select the appropriate frame and hit Enter";
            this.instructionSetAortic[11] = "Click on initial cm marker";
            this.instructionSetAortic[12] = "Click on third subsequent cm marker";
            this.instructionSetAortic[13] = "Measure Aortic diameter: Click on inner edge";
            this.instructionSetAortic[14] = "Measure Aortic diameter: Click subsequent inner edge";
            this.instructionSetAortic[15] = "Press enter to create the report";

            //Instructions for pulmonary
            this.instructionSetPulmonary[0] = "Select the appropriate frame and hit Enter";
            this.instructionSetPulmonary[1] = "Click on initial velocity marker";
            this.instructionSetPulmonary[2] = "Click on second subsequent velocity marker";
            this.instructionSetPulmonary[3] = "Click on initial time marker";
            this.instructionSetPulmonary[4] = "Click on second subsequent time marker";
            this.instructionSetPulmonary[5] = "Click on start of systole";
            this.instructionSetPulmonary[6] = "Click on subsequent start of systole";
            this.instructionSetPulmonary[7] = "Trace pulmonary flow velocity contour and hit Enter to close the region";
            this.instructionSetPulmonary[8] = "Hit Enter to continue..";
            this.instructionSetPulmonary[10] = "Select the appropriate frame and hit Enter";
            this.instructionSetPulmonary[11] = "Click on initial cm marker";
            this.instructionSetPulmonary[12] = "Click on third subsequent cm marker";
            this.instructionSetPulmonary[13] = "Measure pulmonary diameter: Click on inner edge";
            this.instructionSetPulmonary[14] = "Measure pulmonary diameter: Click subsequent inner edge";
            this.instructionSetPulmonary[15] = "Press enter to create the report";

            //Instructions for SVC
            this.instructionSetSVC[0] = "Select the appropriate frame and hit Enter";
            this.instructionSetSVC[1] = "Click on initial velocity marker";
            this.instructionSetSVC[2] = "Click on second subsequent velocity marker";
            this.instructionSetSVC[3] = "Click on initial time marker";
            this.instructionSetSVC[4] = "Click on second subsequent time marker";
            this.instructionSetSVC[5] = "Click on start of systole";
            this.instructionSetSVC[6] = "Click on subsequent start of systole";
            this.instructionSetSVC[7] = "Trace SVC flow velocity contour and hit Enter to close the region";
            this.instructionSetSVC[8] = "Hit Enter to continue..";
            this.instructionSetSVC[10] = "Select the appropriate frame and hit Enter";
            this.instructionSetSVC[11] = "Click on initial cm marker";
            this.instructionSetSVC[12] = "Click on third subsequent cm marker";
            this.instructionSetSVC[13] = "Measure SVC diameter: Click on inner edge";
            this.instructionSetSVC[14] = "Measure SVC diameter: Click subsequent inner edge";
            this.instructionSetSVC[15] = "Press enter to create the report";

            //Instructions for PDA
            this.instructionSetPDA[0] = "Press enter to start calculation of PDA size";
            this.instructionSetPDA[1] = "Click on initial cm marker";
            this.instructionSetPDA[2] = "Click on 3rd subsequent cm marker";
            this.instructionSetPDA[3] = "Click on initial edge of PDA";
            this.instructionSetPDA[4] = "Click on subsequent edge of PDA";
            this.instructionSetPDA[5] = "Press enter to create the report";

            //Instructions for RV Pressure
            this.instructionSetRVPressure[0] = "Press enter to start calculation of RV Pressure";
            this.instructionSetRVPressure[1] = "Click on initial cm marker";
            this.instructionSetRVPressure[2] = "Click on third subsequent cm marker";
            this.instructionSetRVPressure[3] = "Click on baseline pressure"; 
            this.instructionSetRVPressure[4] = "Click on peak of TR jet";
            this.instructionSetRVPressure[5] = "Press enter to create the report";
            
        }

        private void CalcateArea()
        {
            Point midPoint;
            int totalX = 0, totalY = 0;

            foreach (Point point in this.pointsClicked)
            {
                totalX += point.X;
                totalY += point.Y;
            }

            midPoint = new Point(totalX / this.pointsClicked.Count, totalY / this.pointsClicked.Count);

            this.FloodFill(this.imgFrame, midPoint.X, midPoint.Y, Color.Black);

            this.pointsClicked.Clear();
        }

        private void FloodFill(Bitmap bitmap, int x, int y, Color color)
        {
            int countArea = 0;
            BitmapData data = bitmap.LockBits(
                new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            int[] bits = new int[data.Stride / 4 * data.Height];
            Marshal.Copy(data.Scan0, bits, 0, bits.Length);

            LinkedList<Point> check = new LinkedList<Point>();
            int floodTo = color.ToArgb();
            int floodFrom = bits[x + y * data.Stride / 4];
            bits[x + y * data.Stride / 4] = floodTo;

            if (floodFrom != floodTo)
            {
                check.AddLast(new Point(x, y));
                while (check.Count > 0)
                {
                    Point cur = check.First.Value;
                    check.RemoveFirst();

                    foreach (Point off in new Point[] {
                new Point(0, -1), new Point(0, 1), 
                new Point(-1, 0), new Point(1, 0)})
                    {
                        Point next = new Point(cur.X + off.X, cur.Y + off.Y);
                        if (next.X >= 0 && next.Y >= 0 &&
                            next.X < data.Width &&
                            next.Y < data.Height)
                        {
                            if (bits[next.X + next.Y * data.Stride / 4] == floodFrom)
                            {
                                check.AddLast(next);
                                bits[next.X + next.Y * data.Stride / 4] = floodTo;
                                countArea++;
                            }
                        }
                    }
                }
            }

            Marshal.Copy(bits, 0, data.Scan0, bits.Length);
            bitmap.UnlockBits(data);

            //bitmap.Save("C:\\imgflooded" + DateTime.Now.Millisecond + ".bmp");

            MainForm.areaInPixels.Add(countArea);
        }

        private void FormLoad(object sender, EventArgs e)
        {
            this.mouse_key_input_counter = 0;

            this.calculationsDialogue.ShowDialog(this);
            MainForm.areaInPixels.Clear();

            try
            {
                int width, height;
                width = (this.pictureBox1.Width / 4) * 4;
                height = (this.pictureBox1.Height / 4) * 4;

                switch (calculationMode)
                {
                    case CalculationModes.LV:
                        this.currentVideoPath = "C:\\VETS Source Code\\TrakStar Application Data\\Videos\\79.avi";
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetLV[this.mouse_key_input_counter++];

                        break;

                    case CalculationModes.LA_AO:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetLA_AO[this.mouse_key_input_counter++];
                        break;

                    case CalculationModes.TwoDim_LV_Volume:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSet2D[this.mouse_key_input_counter++];

                        break;

                    case CalculationModes.Aortic_Blood_Flow:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                        break;

                    case CalculationModes.Pulmonary_Blood_Flow:
                        this.currentVideoPath = "C:\\VETS Source Code\\TrakStar Application Data\\Videos\\54.avi";
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                        break;

                    case CalculationModes.SVC_Blood_Flow:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                        break;

                    case CalculationModes.PDA:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                        break;

                    case CalculationModes.RVPressure:
                        this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                            (this.currentVideoPath, 1.0, new Size(width, height));
                        this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                        break;
                }
            }

            catch (FrameGrabber.InvalidVideoFileException ex)
            {
                MessageBox.Show(ex.Message, "Extraction failed");
            }
            catch (StackOverflowException)
            {
                MessageBox.Show("The target image size is too big", "Extraction failed");
            }
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            switch(calculationMode)
            {
                case CalculationModes.LV:

                    #region LV Mode Instruction Set
                    switch (this.mouse_key_input_counter)
                    {
                        case 0:
                            //Do nothing
                            break;

                        case 1:
                            this.calculationPackage.y_dist[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[2];
                            this.mouse_key_input_counter++;
                            break;

                        case 2:
                            this.calculationPackage.y_dist[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[3];
                            this.mouse_key_input_counter++;
                            break;

                        case 3:
                            this.calculationPackage.x_time[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[4];
                            this.mouse_key_input_counter++;
                            break;

                        case 4:
                            this.calculationPackage.x_time[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[5];
                            this.mouse_key_input_counter++;
                            break;

                        case 5:
                            this.calculationPackage.mouseCoordinates[5] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[7];
                            this.mouse_key_input_counter++;
                            break;

                        case 6:
                            this.calculationPackage.mouseCoordinates[6] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[8];
                            this.mouse_key_input_counter++;
                            break;

                        case 7:
                            this.calculationPackage.mouseCoordinates[7] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[9];
                            this.mouse_key_input_counter++;
                            break;

                        case 8:
                            this.calculationPackage.mouseCoordinates[8] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[10];
                            this.mouse_key_input_counter++;
                            break;

                        case 9:
                            this.calculationPackage.mouseCoordinates[9] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[11];
                            this.mouse_key_input_counter++;
                            break;

                        case 10:
                            this.calculationPackage.mouseCoordinates[10] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[12];
                            this.mouse_key_input_counter++;
                            break;

                        case 11:
                            this.calculationPackage.mouseCoordinates[11] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[13];
                            this.mouse_key_input_counter++;
                            break;

                        case 12:
                            this.calculationPackage.mouseCoordinates[12] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[14];
                            this.mouse_key_input_counter++;
                            break;

                        case 13:
                            this.calculationPackage.mouseCoordinates[13] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[15];
                            this.mouse_key_input_counter++;
                            break;

                        case 14:
                            this.calculationPackage.mouseCoordinates[14] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLV[16];
                            this.mouse_key_input_counter++;
                            break;

                        case 15:
                            this.calculationPackage.mouseCoordinates[15] = e.Location;
                            this.lbInstructions.Text = "Press enter to create report";

                            break;

                        default:
                            //Do nothing?
                            break;
                    }
                    {
                        this.lastScreenShot = this.pictureBox1.Image;
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(
                            e.X - 5, e.Y - 5, 12, 12);
                        graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);                        
                    }


                    break;

                #endregion

                case CalculationModes.LA_AO:

                    #region LA_AO Mode Instruction Set

                    switch (this.mouse_key_input_counter)
                    {
                        case 0:
                            //Do nothing
                            break;

                        case 1:
                            this.calculationPackage.y_dist[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLA_AO[2];
                            this.mouse_key_input_counter++;
                            break;

                        case 2:
                            this.calculationPackage.y_dist[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLA_AO[3];
                            this.mouse_key_input_counter++;
                            break;

                        case 3:
                            this.calculationPackage.mouseCoordinates[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLA_AO[5];
                            this.mouse_key_input_counter++;
                            break;

                        case 4:
                            this.calculationPackage.mouseCoordinates[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetLA_AO[6];
                            this.mouse_key_input_counter++;
                            break;

                        case 5:
                            this.calculationPackage.mouseCoordinates[2] = e.Location;
                            this.lbInstructions.Text = "Press enter to create report";
                            break;

                        default:
                            //Do nothing?
                            break;
                    }
                    {
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(
                            e.X - 5, e.Y - 5, 12, 12);
                        graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                    }

                    break;

                #endregion

                case CalculationModes.TwoDim_LV_Volume:
                    #region 2D Volume Instruction Set
                    if (this.mouse_key_input_counter == 1)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[this.mouse_key_input_counter++];

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(
                                e.X - 5, e.Y - 5, 12, 12);
                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                        }
                    }


                    else if (this.mouse_key_input_counter == 2)
                    {
                        this.calculationPackage.y_dist[1] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[this.mouse_key_input_counter++];

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(
                                e.X - 5, e.Y - 5, 12, 12);
                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                        }
                    }

                    else if (this.mouse_key_input_counter == 4 || this.mouse_key_input_counter == 13)
                    {
                        //Drawing the image
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), e.Location.X, e.Location.Y, 1, 1);

                        this.lastPointClicked = this.startLocation = e.Location;
                        this.imgFrame = new Bitmap(this.pictureBox1.Width, this.pictureBox1.Height);
                        this.pointsClicked.Add(e.Location);

                        this.mouse_key_input_counter++;
                    }

                    else if (this.mouse_key_input_counter == 5 || this.mouse_key_input_counter == 14)
                    {
                        //Drawing onto the screen
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);

                        //Drawing onto the image
                        graphics = Graphics.FromImage(this.imgFrame);
                        rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);

                        this.lastPointClicked = e.Location;
                        this.pointsClicked.Add(e.Location);
                    }

                    else if (this.mouse_key_input_counter == 6 || this.mouse_key_input_counter == 15)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[5];
                        this.mouse_key_input_counter++;

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(e.X, e.Y, 1, 1);
                            graphics.DrawEllipse(new Pen(Color.Blue, 2f), rectangle);
                        }

                        this.lastPointClicked = e.Location;
                    }

                    else if (this.mouse_key_input_counter == 7 || this.mouse_key_input_counter == 16)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[6];
                        this.mouse_key_input_counter++;

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(e.X, e.Y, 1, 1);
                            graphics.DrawEllipse(new Pen(Color.Blue, 2f), rectangle);
                            graphics.DrawLine(new Pen(Color.Blue, 2f), this.lastPointClicked, e.Location);
                        }

                        MainForm.lengthsInPixels.Add(Math.Sqrt(Math.Pow(this.lastPointClicked.Y - e.Location.Y,2) + Math.Pow(this.lastPointClicked.X - e.Location.X,2)));
                        //this.calculationPackage.Calculate();
                        this.lastPointClicked = e.Location;
                    }

                    else if (this.mouse_key_input_counter == 8 || this.mouse_key_input_counter == 17)
                    {
                        //Drawing the image
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), e.Location.X, e.Location.Y, 1, 1);

                        this.lastPointClicked = this.startLocation = e.Location;
                        this.imgFrame = new Bitmap(this.pictureBox1.Width, this.pictureBox1.Height);
                        this.pointsClicked.Add(e.Location);

                        this.mouse_key_input_counter++;
                    }

                    else if (this.mouse_key_input_counter == 9 || this.mouse_key_input_counter == 18)
                    {
                        //Drawing onto the screen
                        Graphics graphics = this.pictureBox1.CreateGraphics();
                        Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);

                        //Drawing onto the image
                        graphics = Graphics.FromImage(this.imgFrame);
                        rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                        graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);

                        this.lastPointClicked = e.Location;
                        this.pointsClicked.Add(e.Location);
                    }

                    else if (this.mouse_key_input_counter == 10 || this.mouse_key_input_counter == 19)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[8];
                        this.mouse_key_input_counter++;

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(e.X, e.Y, 1, 1);
                            graphics.DrawEllipse(new Pen(Color.Blue, 2f), rectangle);
                        }

                        this.lastPointClicked = e.Location;
                    }

                    else if (this.mouse_key_input_counter == 11)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[9];
                        this.mouse_key_input_counter++;

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(e.X, e.Y, 1, 1);
                            graphics.DrawEllipse(new Pen(Color.Blue, 2f), rectangle);
                            graphics.DrawLine(new Pen(Color.Blue, 2f), this.lastPointClicked, e.Location);
                        }
                        MainForm.lengthsInPixels.Add(Math.Sqrt(Math.Pow(this.lastPointClicked.Y - e.Location.Y, 2) + Math.Pow(this.lastPointClicked.X - e.Location.X, 2)));
                        
                        this.lastPointClicked = e.Location;
                    }

                    else if (this.mouse_key_input_counter == 20)
                    {
                        this.calculationPackage.y_dist[0] = e.Location;
                        this.lbInstructions.Text = this.instructionSet2D[16];
                        this.mouse_key_input_counter++;

                        {
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(e.X, e.Y, 1, 1);
                            graphics.DrawEllipse(new Pen(Color.Blue, 2f), rectangle);
                            graphics.DrawLine(new Pen(Color.Blue, 2f), this.lastPointClicked, e.Location);
                        }
                        MainForm.lengthsInPixels.Add(Math.Sqrt(Math.Pow(this.lastPointClicked.Y - e.Location.Y, 2) + Math.Pow(this.lastPointClicked.X - e.Location.X, 2)));
                        
                        this.lastPointClicked = e.Location;
                    }
                    break;

                    #endregion

                case CalculationModes.Aortic_Blood_Flow:
                    #region Aortic
                    switch (this.mouse_key_input_counter)
                    {
                        case 2:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.velocity[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 3:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.velocity[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 4:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.time[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 5:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.time[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 6:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.HR[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 7:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.HR[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case 8:
                            //Drawing the image
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), e.Location.X, e.Location.Y, 1, 1);
                            }

                            this.lastPointClicked = this.startLocation = e.Location;
                            this.imgFrame = new Bitmap(this.pictureBox1.Width, this.pictureBox1.Height);
                            this.pointsClicked.Add(e.Location);

                            this.mouse_key_input_counter++;
                            break;

                        case 9:
                            {
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);


                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);
                            }
                            this.lastPointClicked = e.Location;

                            this.pointsClicked.Add(e.Location);
                            break;

                        case 12:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.distance[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                            break;

                        case 13:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.distance[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                            break;

                        case 14:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.diameter[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                            break;

                        case 15:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.aorticInfo.diameter[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                            break;

                    }
                    break;
                    #endregion

                case CalculationModes.Pulmonary_Blood_Flow:
                    #region Pulmonary
                    switch (this.mouse_key_input_counter)
                    {
                        case 2:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.velocity[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 3:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.velocity[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 4:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.time[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 5:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.time[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 6:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.HR[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 7:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.HR[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                            break;

                        case 8:
                            //Drawing the image
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), e.Location.X, e.Location.Y, 1, 1);
                            }

                            this.lastPointClicked = this.startLocation = e.Location;
                            this.imgFrame = new Bitmap(this.pictureBox1.Width, this.pictureBox1.Height);
                            this.pointsClicked.Add(e.Location);

                            this.mouse_key_input_counter++;
                            break;

                        case 9:
                            {
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);


                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);
                            }
                            this.lastPointClicked = e.Location;
                            this.pointsClicked.Add(e.Location);
                            break;

                        case 12:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.distance[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                        case 13:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.distance[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                        case 14:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.diameter[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                        case 15:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pulmonaryInfo.diameter[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                    }
                    break;
                    #endregion

                case CalculationModes.SVC_Blood_Flow:
                    #region SVC
                    switch (this.mouse_key_input_counter)
                    {
                        case 2:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.velocity[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 3:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.velocity[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 4:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.time[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 5:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.time[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 6:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.HR[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 7:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.HR[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;

                        case 8:
                            //Drawing the image
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), e.Location.X, e.Location.Y, 1, 1);
                            }

                            this.lastPointClicked = this.startLocation = e.Location;
                            this.imgFrame = new Bitmap(this.pictureBox1.Width, this.pictureBox1.Height);
                            this.pointsClicked.Add(e.Location);

                            this.mouse_key_input_counter++;
                            break;

                        case 9:
                            {
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);


                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(e.X - 5, e.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, e.Location);
                            }
                            this.lastPointClicked = e.Location;
                            this.pointsClicked.Add(e.Location);
                            break;

                        case 12:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.distance[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;

                        case 13:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.distance[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;

                        case 14:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.diameter[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;

                        case 15:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.svcInfo.diameter[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;

                    }
                    break;
                    #endregion

                case CalculationModes.PDA:
                    #region PDA Instruction Set
                    switch (this.mouse_key_input_counter)
                    {
                        case 0:
                            //Do nothing
                            break;

                        case 2:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.y_dist[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                            break;

                        case 3:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.y_dist[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];                       
                            break;

                        case 4:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pdaInfo.diameter[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                            break;

                        case 5:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.pdaInfo.diameter[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                            break;
                    }
                    
                    #endregion
                    break;

                case CalculationModes.RVPressure:
                    #region RV Pressure Instruction Set
                    switch (this.mouse_key_input_counter)
                    {
                        case 0:
                            //Do nothing
                            break;

                        case 2:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.y_dist[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetRVPressure[this.mouse_key_input_counter++];
                            break;

                        case 3:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.y_dist[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetRVPressure[this.mouse_key_input_counter++];
                            break;

                        case 4:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.rvInfo.pressure_tr_jet[0] = e.Location;
                            this.lbInstructions.Text = this.instructionSetRVPressure[this.mouse_key_input_counter++];
                            break;

                        case 5:
                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(
                                    e.X - 5, e.Y - 5, 12, 12);
                                graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                            }

                            this.calculationPackage.rvInfo.pressure_tr_jet[1] = e.Location;
                            this.lbInstructions.Text = this.instructionSetRVPressure[this.mouse_key_input_counter++];
                            break;
                    }

                    #endregion
                    break;
            }
            
        }

        //If stand-alone
        /*private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                switch (calculationMode)
                {
                    case CalculationModes.LV:
                        #region LV
                        if (this.mouse_key_input_counter == 1)
                            this.lbInstructions.Text = this.instructionSetLV[1];
                        else if (this.mouse_key_input_counter == 5)
                        {
                            this.lbInstructions.Text = this.instructionSetLV[6];
                        }
                        else if (this.mouse_key_input_counter == 15)
                        {
                            this.calculationPackage.Calculate();
                            this.fullReport.lbHeartRate.Text = this.calculationPackage.HR.ToString("N2");
                            this.fullReport.lbLVPWs.Text = this.calculationPackage.LVPWs.ToString("N2");
                            this.fullReport.lbLVIDs.Text = this.calculationPackage.LVIDs.ToString("N2");
                            this.fullReport.lbLVIDd.Text = this.calculationPackage.LVIDd.ToString("N2");
                            this.fullReport.lbIVs.Text = this.calculationPackage.IVSs.ToString("N2");
                            this.fullReport.lbIVSd.Text = this.calculationPackage.IVSd.ToString("N2");
                            this.fullReport.lbRVIDd.Text = this.calculationPackage.RVIDd.ToString("N2");
                            this.fullReport.lbAOs.Text = this.calculationPackage.AOs.ToString("N2");
                            this.fullReport.lbLAs.Text = this.calculationPackage.LAs.ToString("N2");
                            this.fullReport.lbLA_AO.Text = this.calculationPackage.LA_AO.ToString("N2");
                            this.fullReport.lbEDV.Text = this.calculationPackage.EDV.ToString("N2");
                            this.fullReport.lbLVMass.Text = this.calculationPackage.LVmass.ToString("N2");
                            this.fullReport.lbIVSThickening.Text = this.calculationPackage.IVSthickening.ToString("N2");
                            this.fullReport.lbFS.Text = this.calculationPackage.FS.ToString("N2");
                            this.fullReport.lbESV.Text = this.calculationPackage.ESV.ToString("N2");
                            this.fullReport.lbLVSV.Text = this.calculationPackage.LVSV.ToString("N2");
                            this.fullReport.lbLVEF.Text = this.calculationPackage.LVEF.ToString("N2");
                            this.fullReport.lbLVPWThickening.Text = this.calculationPackage.LVPWthickening.ToString("N2");
                            this.fullReport.lbLVPWd.Text = this.calculationPackage.LVPWd.ToString("N2");
                            this.fullReport.lbIVSd_LVPWd.Text = (this.calculationPackage.IVSd / this.calculationPackage.LVPWd).ToString("N2");
                            this.fullReport.lbLVMass_kg.Text = (this.calculationPackage.LVmass / (double)this.calculationsDialogue.nudWeight.Value).ToString("N2");
                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }

                        break;
                        #endregion

                    case CalculationModes.LA_AO:
                        #region LA/AO
                        if (this.mouse_key_input_counter == 1)
                            this.lbInstructions.Text = this.instructionSetLA_AO[1];
                        else if (this.mouse_key_input_counter == 3)
                        {
                            this.lbInstructions.Text = this.instructionSetLA_AO[4];
                        }
                        else if (this.mouse_key_input_counter == 5)
                        {
                            this.calculationPackage.Calculate();

                            this.fullReport.lbHeartRate.Text = "NIL";
                            this.fullReport.lbLVPWs.Text = "NIL";
                            this.fullReport.lbLVIDs.Text = "NIL";
                            this.fullReport.lbLVIDd.Text = "NIL";
                            this.fullReport.lbIVs.Text = "NIL";
                            this.fullReport.lbIVSd.Text = "NIL";
                            this.fullReport.lbRVIDd.Text = "NIL";

                            this.fullReport.lbEDV.Text = "NIL";
                            this.fullReport.lbLVMass.Text = "NIL";
                            this.fullReport.lbIVSThickening.Text = "NIL";
                            this.fullReport.lbFS.Text = "NIL";
                            this.fullReport.lbESV.Text = "NIL";
                            this.fullReport.lbLVSV.Text = "NIL";
                            this.fullReport.lbLVEF.Text = "NIL";
                            this.fullReport.lbLVPWThickening.Text = "NIL";
                            this.fullReport.lbLVPWd.Text = "NIL";
                            this.fullReport.lbIVSd_LVPWd.Text = "NIL";
                            this.fullReport.lbLVMass_kg.Text = "NIL";

                            this.fullReport.lbAOs.Text = this.calculationPackage.AOs.ToString("N2");
                            this.fullReport.lbLAs.Text = this.calculationPackage.LAs.ToString("N2");
                            this.fullReport.lbLA_AO.Text = this.calculationPackage.LA_AO.ToString("N2");

                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }

                        break;
                        #endregion

                    case CalculationModes.TwoDim_LV_Volume:
                        #region 2D
                        if (this.mouse_key_input_counter == 3)
                        {
                            this.lbInstructions.Text = this.instructionSet2D[3];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 5 || this.mouse_key_input_counter == 14)
                        {
                            //Drawing onto the screen
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //Drawing onto the image
                            graphics = Graphics.FromImage(this.imgFrame);
                            rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //this.imgFrame.Save("C:\\img" + DateTime.Now.Millisecond + ".bmp");

                            this.CalcateArea();

                            this.lbInstructions.Text = this.instructionSet2D[4];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 9 || this.mouse_key_input_counter == 18)
                        {
                            //Drawing onto the screen
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //Drawing onto the image
                            graphics = Graphics.FromImage(this.imgFrame);
                            rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //this.imgFrame.Save("C:\\img" + DateTime.Now.Millisecond + ".bmp");

                            this.CalcateArea();

                            this.lbInstructions.Text = this.instructionSet2D[7];
                            this.mouse_key_input_counter++;
                        }
                            
                        else if (this.mouse_key_input_counter == 12)
                        {
                            this.lbInstructions.Text = this.instructionSet2D[10];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 21)
                        {
                            this.calculationPackage.Calculate();

                            this.fullReport.lb2dEDV.Text = this.calculationPackage.twoDimInfo.EDV.ToString("N2");
                            this.fullReport.lb2dESV.Text = this.calculationPackage.twoDimInfo.ESV.ToString("N2");
                            this.fullReport.lb2dLVMass.Text = this.calculationPackage.twoDimInfo.LVMass.ToString("N2");
                            this.fullReport.lb2dMassPerKg.Text = this.calculationPackage.twoDimInfo.LVMassPerKg.ToString("N2");
                            this.fullReport.lb2dSV.Text = this.calculationPackage.twoDimInfo.SV.ToString("N2");
                            
                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }
                        break;
                        #endregion

                    case CalculationModes.Aortic_Blood_Flow:
                        #region Aortic
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetAortic[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.aorticInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.aorticInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\01.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                }
                                     
                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                break;

                            case 15:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = this.calculationPackage.aorticInfo.bloodFlow.ToString("N2");
                                this.fullReport.lbA_SV.Text = this.calculationPackage.aorticInfo.SV.ToString("N2");
                                this.fullReport.lbP_BFlow.Text = "NIL";
                                this.fullReport.lbP_SV.Text = "NIL";
                                this.fullReport.lbSVCFlow.Text = "NIL";
                                this.fullReport.lbSVCSV.Text = "NIL";

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }
                        break;
                        #endregion

                    case CalculationModes.Pulmonary_Blood_Flow:
                        #region Pulmonary
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetPulmonary[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.pulmonaryInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.pulmonaryInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\02.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                                }

                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                break;

                            case 15:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = "NIL";
                                this.fullReport.lbA_SV.Text = "NIL";
                                this.fullReport.lbP_BFlow.Text = this.calculationPackage.pulmonaryInfo.bloodFlow.ToString("N2"); ;
                                this.fullReport.lbP_SV.Text = this.calculationPackage.pulmonaryInfo.SV.ToString("N2"); ;
                                this.fullReport.lbSVCFlow.Text = "NIL";
                                this.fullReport.lbSVCSV.Text = "NIL";

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }
                        break;
                        #endregion

                    case CalculationModes.SVC_Blood_Flow:
                        #region SVC
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetSVC[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.svcInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.svcInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\26.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                                }

                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                break;

                            case 15:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = "NIL";
                                this.fullReport.lbA_SV.Text = "NIL";
                                this.fullReport.lbP_SV.Text = "NIL";
                                this.fullReport.lbP_BFlow.Text = "NIL";
                                this.fullReport.lbSVCFlow.Text = this.calculationPackage.svcInfo.bloodFlow.ToString("N2");
                                this.fullReport.lbSVCSV.Text = this.calculationPackage.svcInfo.SV.ToString("N2");

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }

                        break;
                        #endregion

                    default:
                        break;
                }
            }

            else if (e.KeyCode == Keys.C)
            {
                this.mouse_key_input_counter = 0;

                this.calculationsDialogue.ShowDialog(this);
                MainForm.areaInPixels.Clear();

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\79.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetLV[this.mouse_key_input_counter++];
                            
                                break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\84.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetLA_AO[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\35.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSet2D[this.mouse_key_input_counter++];
                         
                            break;

                        case CalculationModes.Aortic_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\62.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\75.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\70.avi", 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                            break;
                    }
                }

                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }

                
            }

            else if (e.KeyCode == Keys.Left)
            {
                this.percentPosition -= 0.05f;

                if (this.percentPosition < 0.0)
                    this.percentPosition = 1.0;

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\79.avi", this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\84.avi", this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\35.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Aortic_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\62.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\75.avi", this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\70.avi", this.percentPosition, new Size(width, height));
                            break;
                    }
                }
                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }
            }

            else if (e.KeyCode == Keys.Right)
            {
                this.percentPosition += 0.05f;

                if (this.percentPosition > 1.0)
                    this.percentPosition = 0.0;

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\79.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\84.avi", this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\35.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Aortic_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\62.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\75.avi", this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\70.avi", this.percentPosition, new Size(width, height));
                            break;
                    }
                }
                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }
            }

            else if (e.KeyCode == Keys.Q)
            {
                Application.Exit();
            }

        }*/

        //If connected to 2D directly
        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                switch (calculationMode)
                {
                    case CalculationModes.LV:
                        #region LV
                        if (this.mouse_key_input_counter == 1)
                            this.lbInstructions.Text = this.instructionSetLV[1];
                        else if (this.mouse_key_input_counter == 5)
                        {
                            this.lbInstructions.Text = this.instructionSetLV[6];
                        }
                        else if (this.mouse_key_input_counter == 15)
                        {
                            this.calculationPackage.Calculate();
                            this.fullReport.lbHeartRate.Text = this.calculationPackage.HR.ToString();
                            this.fullReport.lbLVPWs.Text = this.calculationPackage.LVPWs.ToString("N2");
                            this.fullReport.lbLVIDs.Text = this.calculationPackage.LVIDs.ToString("N2");
                            this.fullReport.lbLVIDd.Text = this.calculationPackage.LVIDd.ToString("N2");
                            this.fullReport.lbIVs.Text = this.calculationPackage.IVSs.ToString("N2");
                            this.fullReport.lbIVSd.Text = this.calculationPackage.IVSd.ToString("N2");
                            this.fullReport.lbRVIDd.Text = this.calculationPackage.RVIDd.ToString("N2");
                            /*this.fullReport.lbAOs.Text = this.calculationPackage.AOs.ToString("N2");
                            this.fullReport.lbLAs.Text = this.calculationPackage.LAs.ToString("N2");
                            this.fullReport.lbLA_AO.Text = this.calculationPackage.LA_AO.ToString("N2");*/
                            this.fullReport.lbEDV.Text = this.calculationPackage.EDV.ToString("N2");
                            this.fullReport.lbLVMass.Text = this.calculationPackage.LVmass.ToString("N2");
                            this.fullReport.lbIVSThickening.Text = this.calculationPackage.IVSthickening.ToString("N2");
                            this.fullReport.lbFS.Text = this.calculationPackage.FS.ToString("N2");
                            this.fullReport.lbESV.Text = this.calculationPackage.ESV.ToString("N2");
                            this.fullReport.lbLVSV.Text = this.calculationPackage.LVSV.ToString("N2");
                            this.fullReport.lbLVEF.Text = this.calculationPackage.LVEF.ToString("N2");
                            this.fullReport.lbLVPWThickening.Text = this.calculationPackage.LVPWthickening.ToString("N2");
                            this.fullReport.lbLVPWd.Text = this.calculationPackage.LVPWd.ToString("N2");
                            this.fullReport.lbIVSd_LVPWd.Text = (this.calculationPackage.IVSd / this.calculationPackage.LVPWd).ToString("N2");
                            this.fullReport.lbLVMass_kg.Text = (this.calculationPackage.LVmass / (double)this.calculationsDialogue.nudWeight.Value).ToString("N2");
                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }

                        break;
                        #endregion

                    case CalculationModes.LA_AO:
                        #region LA/AO
                        if (this.mouse_key_input_counter == 1)
                            this.lbInstructions.Text = this.instructionSetLA_AO[1];
                        else if (this.mouse_key_input_counter == 3)
                        {
                            this.lbInstructions.Text = this.instructionSetLA_AO[4];
                        }
                        else if (this.mouse_key_input_counter == 5)
                        {
                            this.calculationPackage.Calculate();

                            /*this.fullReport.lbHeartRate.Text = "NIL";
                            this.fullReport.lbLVPWs.Text = "NIL";
                            this.fullReport.lbLVIDs.Text = "NIL";
                            this.fullReport.lbLVIDd.Text = "NIL";
                            this.fullReport.lbIVs.Text = "NIL";
                            this.fullReport.lbIVSd.Text = "NIL";
                            this.fullReport.lbRVIDd.Text = "NIL";

                            this.fullReport.lbEDV.Text = "NIL";
                            this.fullReport.lbLVMass.Text = "NIL";
                            this.fullReport.lbIVSThickening.Text = "NIL";
                            this.fullReport.lbFS.Text = "NIL";
                            this.fullReport.lbESV.Text = "NIL";
                            this.fullReport.lbLVSV.Text = "NIL";
                            this.fullReport.lbLVEF.Text = "NIL";
                            this.fullReport.lbLVPWThickening.Text = "NIL";
                            this.fullReport.lbLVPWd.Text = "NIL";
                            this.fullReport.lbIVSd_LVPWd.Text = "NIL";
                            this.fullReport.lbLVMass_kg.Text = "NIL";*/

                            this.fullReport.lbAOs.Text = this.calculationPackage.AOs.ToString("N2");
                            this.fullReport.lbLAs.Text = this.calculationPackage.LAs.ToString("N2");
                            this.fullReport.lbLA_AO.Text = this.calculationPackage.LA_AO.ToString("N2");

                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }

                        break;
                        #endregion

                    case CalculationModes.TwoDim_LV_Volume:
                        #region 2D
                        if (this.mouse_key_input_counter == 3)
                        {
                            this.lbInstructions.Text = this.instructionSet2D[3];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 5 || this.mouse_key_input_counter == 14)
                        {
                            //Drawing onto the screen
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //Drawing onto the image
                            graphics = Graphics.FromImage(this.imgFrame);
                            rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //this.imgFrame.Save("C:\\img" + DateTime.Now.Millisecond + ".bmp");

                            this.CalcateArea();

                            this.lbInstructions.Text = this.instructionSet2D[4];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 9 || this.mouse_key_input_counter == 18)
                        {
                            //Drawing onto the screen
                            Graphics graphics = this.pictureBox1.CreateGraphics();
                            Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //Drawing onto the image
                            graphics = Graphics.FromImage(this.imgFrame);
                            rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                            graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                            graphics.Dispose();

                            //this.imgFrame.Save("C:\\img" + DateTime.Now.Millisecond + ".bmp");

                            this.CalcateArea();

                            this.lbInstructions.Text = this.instructionSet2D[7];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 12)
                        {
                            this.lbInstructions.Text = this.instructionSet2D[10];
                            this.mouse_key_input_counter++;
                        }

                        else if (this.mouse_key_input_counter == 21)
                        {
                            this.calculationPackage.Calculate();

                            this.fullReport.lb2dEDV.Text = this.calculationPackage.twoDimInfo.EDV.ToString("N2");
                            this.fullReport.lb2dESV.Text = this.calculationPackage.twoDimInfo.ESV.ToString("N2");
                            this.fullReport.lb2dLVMass.Text = this.calculationPackage.twoDimInfo.LVMass.ToString("N2");
                            this.fullReport.lb2dMassPerKg.Text = this.calculationPackage.twoDimInfo.LVMassPerKg.ToString("N2");
                            this.fullReport.lb2dSV.Text = this.calculationPackage.twoDimInfo.SV.ToString("N2");

                            this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                            this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                            this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                            this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                            this.fullReport.ShowDialog(this);

                            this.lbInstructions.Text = "Completed LV M-Mode calculations.\nPress C to start a new calculation process.";
                        }
                        break;
                        #endregion

                    case CalculationModes.Aortic_Blood_Flow:
                        #region Aortic
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetAortic[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.aorticInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.aorticInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\01.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                                    this.offline_video_4_measurements = true;
                                }

                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                
                                break;

                            case 11:
                                this.offline_video_4_measurements = false;
                                this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                break;

                            case 16:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = this.calculationPackage.aorticInfo.bloodFlow.ToString("N2");
                                this.fullReport.lbA_SV.Text = this.calculationPackage.aorticInfo.SV.ToString("N2");
                                this.fullReport.lbP_BFlow.Text = "NIL";
                                this.fullReport.lbP_SV.Text = "NIL";
                                this.fullReport.lbSVCFlow.Text = "NIL";
                                this.fullReport.lbSVCSV.Text = "NIL";

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }
                        break;
                        #endregion

                    case CalculationModes.Pulmonary_Blood_Flow:
                        #region Pulmonary
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetPulmonary[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.pulmonaryInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.pulmonaryInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\02.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];

                                    this.offline_video_4_measurements = true;
                                }

                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                break;

                            case 11:
                                this.offline_video_4_measurements = false;
                                this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                break;

                            case 16:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = "NIL";
                                this.fullReport.lbA_SV.Text = "NIL";
                                this.fullReport.lbP_BFlow.Text = this.calculationPackage.pulmonaryInfo.bloodFlow.ToString("N2"); ;
                                this.fullReport.lbP_SV.Text = this.calculationPackage.pulmonaryInfo.SV.ToString("N2"); ;
                                this.fullReport.lbSVCFlow.Text = "NIL";
                                this.fullReport.lbSVCSV.Text = "NIL";

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }
                        break;
                        #endregion

                    case CalculationModes.SVC_Blood_Flow:
                        #region SVC
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                                break;

                            case 9:
                                this.lbInstructions.Text = this.instructionSetSVC[8];
                                this.mouse_key_input_counter++;
                                //Drawing onto the screen
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                //Drawing onto the image
                                graphics = Graphics.FromImage(this.imgFrame);
                                rectangle = new Rectangle(this.lastPointClicked.X - 5, this.lastPointClicked.Y - 5, 1, 1);
                                graphics.DrawLine(new Pen(Color.Yellow, 1.5f), this.lastPointClicked, this.startLocation);
                                graphics.Dispose();

                                this.calculationPackage.svcInfo.distStarttoEndClick[0] = this.startLocation;
                                this.calculationPackage.svcInfo.distStarttoEndClick[1] = this.lastPointClicked;

                                this.CalcateArea();
                                break;

                            case 10:
                                try
                                {
                                    int width, height;
                                    width = (this.pictureBox1.Width / 4) * 4;
                                    height = (this.pictureBox1.Height / 4) * 4;
                                    this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\26.avi", 1.0, new Size(width, height));
                                    this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];

                                    this.offline_video_4_measurements = true;
                                }

                                catch (FrameGrabber.InvalidVideoFileException ex)
                                {
                                    MessageBox.Show(ex.Message, "Extraction failed");
                                }
                                catch (StackOverflowException)
                                {
                                    MessageBox.Show("The target image size is too big", "Extraction failed");
                                }
                                break;

                            case 11:
                                this.offline_video_4_measurements = false;
                                this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];
                                break;

                            case 16:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbA_bflow.Text = "NIL";
                                this.fullReport.lbA_SV.Text = "NIL";
                                this.fullReport.lbP_SV.Text = "NIL";
                                this.fullReport.lbP_BFlow.Text = "NIL";
                                this.fullReport.lbSVCFlow.Text = this.calculationPackage.svcInfo.bloodFlow.ToString("N2");
                                this.fullReport.lbSVCSV.Text = this.calculationPackage.svcInfo.SV.ToString("N2");

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }

                        break;
                        #endregion

                    case CalculationModes.PDA:
                        #region PDA
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                                break;

                            case 6:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lbPDADiameter.Text = this.calculationPackage.pdaInfo.PDA_diameter.ToString("N2");

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;
                        }
                        break;
                        #endregion

                    case CalculationModes.RVPressure:
                        #region RV Pressure
                        switch (this.mouse_key_input_counter)
                        {
                            case 1:
                                this.lbInstructions.Text = this.instructionSetRVPressure[this.mouse_key_input_counter++];
                                break;

                            case 6:
                                this.calculationPackage.Calculate();

                                this.fullReport.lbAge.Text = this.calculationsDialogue.nudAge.Value.ToString();
                                this.fullReport.lbName.Text = this.calculationsDialogue.tbName.Text;
                                this.fullReport.lbWeight.Text = this.calculationsDialogue.nudWeight.Value.ToString();
                                this.fullReport.lbID.Text = this.calculationsDialogue.tbID.Text;

                                this.fullReport.lblRVPressure.Text = this.calculationPackage.rvInfo.RV_pressure.ToString("N2");

                                this.fullReport.ShowDialog(this);

                                this.lbInstructions.Text = "Press C to start a new calculation process.";
                                break;

                            default:
                                break;
                        }
                        break;

                        #endregion
                    default:
                        break;
                }
            }

            else if (e.KeyCode == Keys.C)
            {
                this.mouse_key_input_counter = 0;

                this.calculationsDialogue.ShowDialog(this);
                MainForm.areaInPixels.Clear();

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.currentVideoPath = "C:\\VETS Source Code\\TrakStar Application Data\\Videos\\79.avi";
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetLV[this.mouse_key_input_counter++];

                            break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetLA_AO[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSet2D[this.mouse_key_input_counter++];

                            break;

                        case CalculationModes.Aortic_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetAortic[this.mouse_key_input_counter++];

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            this.offline_video_4_measurements = false;
                            this.currentVideoPath = "C:\\VETS Source Code\\TrakStar Application Data\\Videos\\54.avi";
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetPulmonary[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.PDA:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetPDA[this.mouse_key_input_counter++];
                            break;

                        case CalculationModes.RVPressure:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, 1.0, new Size(width, height));
                            this.lbInstructions.Text = this.instructionSetSVC[this.mouse_key_input_counter++];
                            break;
                    }
                }

                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }


            }

            else if (e.KeyCode == Keys.Left)
            {
                this.percentPosition -= 0.05f;

                if (this.percentPosition < 0.0)
                    this.percentPosition = 1.0;

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Aortic_Blood_Flow:

                            if(this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\54.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\01.avi", this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            if (this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\02.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            if (this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\26.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;
                    }
                }
                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }
            }

            else if (e.KeyCode == Keys.Right)
            {
                this.percentPosition += 0.05f;

                if (this.percentPosition > 1.0)
                    this.percentPosition = 0.0;

                try
                {
                    int width, height;
                    width = (this.pictureBox1.Width / 4) * 4;
                    height = (this.pictureBox1.Height / 4) * 4;

                    switch (calculationMode)
                    {
                        case CalculationModes.LV:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.LA_AO:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.TwoDim_LV_Volume:
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Aortic_Blood_Flow:
                            if (this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\01.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));

                            break;

                        case CalculationModes.Pulmonary_Blood_Flow:
                            if (this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\02.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;

                        case CalculationModes.SVC_Blood_Flow:
                            if (this.offline_video_4_measurements)
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                    ("C:\\VETS Source Code\\TrakStar Application Data\\Videos\\26.avi", this.percentPosition, new Size(width, height));

                            else
                                this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                (this.currentVideoPath, this.percentPosition, new Size(width, height));
                            break;
                    }
                }
                catch (FrameGrabber.InvalidVideoFileException ex)
                {
                    MessageBox.Show(ex.Message, "Extraction failed");
                }
                catch (StackOverflowException)
                {
                    MessageBox.Show("The target image size is too big", "Extraction failed");
                }
            }

            else if (e.KeyCode == Keys.Z)
            {
                int width, height;
                width = (this.pictureBox1.Width / 4) * 4;
                height = (this.pictureBox1.Height / 4) * 4;

                switch (calculationMode)
                {
                    case CalculationModes.LV:

                        #region LV Mode Instruction Set
                        if (this.mouse_key_input_counter <= 1)
                            return;
                            
                        this.mouse_key_input_counter -= 1;

                        try
                        {
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                            (this.currentVideoPath, 1.0, new Size(width, height));
                            this.pictureBox1.Refresh();

                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle;

                                for (int i = 0; i < this.mouse_key_input_counter; i++)
                                {
                                    switch (i)
                                    {
                                        case 0:
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[1];
                                            break;

                                        case 1:
                                            rectangle = new Rectangle(this.calculationPackage.y_dist[0].X - 5,
                                                this.calculationPackage.y_dist[0].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[2];
                                            break;

                                        case 2:
                                            rectangle = new Rectangle(this.calculationPackage.y_dist[1].X - 5,
                                                this.calculationPackage.y_dist[1].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[3];
                                            break;

                                        case 3:
                                            rectangle = new Rectangle(this.calculationPackage.x_time[0].X - 5,
                                                this.calculationPackage.x_time[0].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[4];
                                            break;

                                        case 4:
                                            rectangle = new Rectangle(this.calculationPackage.x_time[1].X - 5,
                                                this.calculationPackage.x_time[1].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[5];
                                            break;

                                        case 5:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[5].X - 5,
                                                this.calculationPackage.mouseCoordinates[5].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[7];
                                            break;

                                        case 6:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[6].X - 5,
                                                this.calculationPackage.mouseCoordinates[6].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[8];
                                            break;

                                        case 7:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[7].X - 5,
                                                this.calculationPackage.mouseCoordinates[7].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[9];
                                            break;

                                        case 8:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[8].X - 5,
                                                this.calculationPackage.mouseCoordinates[8].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[10];
                                            break;

                                        case 9:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[9].X - 5,
                                                this.calculationPackage.mouseCoordinates[9].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[11];
                                            break;

                                        case 10:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[10].X - 5,
                                                this.calculationPackage.mouseCoordinates[10].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[12];
                                            break;

                                        case 11:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[11].X - 5,
                                                this.calculationPackage.mouseCoordinates[11].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[13];
                                            break;

                                        case 12:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[12].X - 5,
                                                this.calculationPackage.mouseCoordinates[12].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[14];
                                            break;

                                        case 13:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[13].X - 5,
                                                this.calculationPackage.mouseCoordinates[13].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[15];
                                            break;

                                        case 14:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[14].X - 5,
                                                this.calculationPackage.mouseCoordinates[14].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLV[16];
                                            break;

                                        case 15:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[15].X - 5,
                                                this.calculationPackage.mouseCoordinates[15].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = "Press enter to create report";
                                            break;

                                        default:
                                            break;
                                    }


                                }
                            }
                        }

                        catch (FrameGrabber.InvalidVideoFileException ex)
                        {
                            MessageBox.Show(ex.Message, "Extraction failed");
                        }
                        catch (StackOverflowException)
                        {
                            MessageBox.Show("The target image size is too big", "Extraction failed");
                        }

                        break;

                        #endregion

                    case CalculationModes.LA_AO:

                        #region LA_AO Mode Instruction Set

                        this.mouse_key_input_counter -= 1;

                        try
                        {
                            this.pictureBox1.Image = FrameGrabber.FrameGrabber.GetFrameFromVideo
                                            (this.currentVideoPath, 1.0, new Size(width, height));
                            this.pictureBox1.Refresh();

                            {
                                Graphics graphics = this.pictureBox1.CreateGraphics();
                                Rectangle rectangle;

                                for (int i = 1; i < this.mouse_key_input_counter; i++)
                                {
                                    switch (i)
                                    {
                                        case 1:
                                            rectangle = new Rectangle(this.calculationPackage.y_dist[0].X - 5,
                                                this.calculationPackage.y_dist[0].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLA_AO[2];
                                            break;

                                        case 2:
                                            rectangle = new Rectangle(this.calculationPackage.y_dist[1].X - 5,
                                                this.calculationPackage.y_dist[1].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLA_AO[3];
                                            break;

                                        case 3:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[0].X - 5,
                                                this.calculationPackage.mouseCoordinates[0].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLA_AO[5];
                                            break;

                                        case 4:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[1].X - 5,
                                                this.calculationPackage.mouseCoordinates[1].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = this.instructionSetLA_AO[6];
                                            break;

                                        case 5:
                                            rectangle = new Rectangle(this.calculationPackage.mouseCoordinates[2].X - 5,
                                                this.calculationPackage.mouseCoordinates[2].Y - 5, 12, 12);
                                            graphics.DrawEllipse(new Pen(Color.Yellow, 1.5f), rectangle);
                                            if (i == this.mouse_key_input_counter - 1) this.lbInstructions.Text = "Press enter to create report";
                                            break;

                                    }
                                }
                            }
                        }
                        catch (FrameGrabber.InvalidVideoFileException ex)
                        {
                            MessageBox.Show(ex.Message, "Extraction failed");
                        }
                        catch (StackOverflowException)
                        {
                            MessageBox.Show("The target image size is too big", "Extraction failed");
                        }

                        break;

                        #endregion
                }
            }

            else if (e.KeyCode == Keys.Space)
            {
                this.Close();//Application.Exit();
            }

        }
    }
}
