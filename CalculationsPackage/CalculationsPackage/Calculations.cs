using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace CalculationsPackage
{
    class Calculations
    {
        public int HR;
        public double LVPWs;
        public double LVPWd;
        public double LVIDs;
        public double LVIDd;
        public double IVSs;
        public double IVSd;
        public double RVIDd;
        public double AOs;
        public double LAs;
        public double LA_AO;

        public double EDV;
        public double LVmass;
        public double IVSthickening;
        public double FS;
        public double ESV;
        public double LVSV;
        public double LVEF;
        public double LVPWthickening;

        public TwoDimInfo twoDimInfo;
        public AorticInfo aorticInfo;
        public PulmonaryInfo pulmonaryInfo;
        public SVCInfo svcInfo;
        public PDAInfo pdaInfo;
        public RVPressureInfo rvInfo;
        public Point[] mouseCoordinates;
        public Point[] x_time;
        public Point[] y_dist;

        public Calculations()
        {
            this.mouseCoordinates = new Point[100];
            this.x_time = new Point[2];
            this.y_dist = new Point[2];

            this.twoDimInfo = new TwoDimInfo();
            this.aorticInfo = new AorticInfo();
            this.pulmonaryInfo = new PulmonaryInfo();
            this.svcInfo = new SVCInfo();
            this.pdaInfo = new PDAInfo();
            this.rvInfo = new RVPressureInfo();
        }

        public void Calculate()
        {
            switch (MainForm.calculationMode)
            {
                case CalculationModes.LV:
                    this.LVMode();
                    break;

                case CalculationModes.LA_AO:
                    this.LA_AOMode();
                    break;

                case CalculationModes.TwoDim_LV_Volume:
                    this.TwoDim_LV_Volume();
                    break;

                case CalculationModes.Aortic_Blood_Flow:
                    this.AorticBloodFlow();
                    break;

                case CalculationModes.Pulmonary_Blood_Flow:
                    this.PulmonaryBloodFlow();
                    break;

                case CalculationModes.SVC_Blood_Flow:
                    this.SVCBloodFlow();
                    break;

                case CalculationModes.PDA:
                    this.PDADiameter();
                    break;

                case CalculationModes.RVPressure:
                    this.RVPressure();
                    break;

                default:
                    //Do nothing
                    break;
            }
        }

        private void LVMode()
        {
            double x_tim = Math.Abs(1 / (float)(this.x_time[0].X - this.x_time[1].X));
            double y_dis = Math.Abs(4 / (float)(this.y_dist[0].Y - this.y_dist[1].Y));
            double heart_rate = Math.Abs((60.0 / ((float)this.mouseCoordinates[15].X - (float)this.mouseCoordinates[14].X)) * (float)(this.x_time[0].X - this.x_time[1].X));
            this.HR = (int)heart_rate;
            this.RVIDd = Math.Abs((float)this.mouseCoordinates[6].Y - (float)this.mouseCoordinates[5].Y) * y_dis;
            this.IVSd = Math.Abs((float)this.mouseCoordinates[7].Y - (float)this.mouseCoordinates[6].Y) * y_dis;
            this.LVIDd = Math.Abs((float)this.mouseCoordinates[8].Y - (float)this.mouseCoordinates[7].Y) * y_dis;
            this.LVPWd = Math.Abs((float)this.mouseCoordinates[9].Y - (float)this.mouseCoordinates[8].Y) * y_dis;
            this.IVSs = Math.Abs((float)this.mouseCoordinates[11].Y - (float)this.mouseCoordinates[10].Y) * y_dis;
            this.LVIDs = Math.Abs((float)this.mouseCoordinates[12].Y - (float)this.mouseCoordinates[11].Y) * y_dis;
            this.LVPWs = Math.Abs((float)this.mouseCoordinates[13].Y - (float)this.mouseCoordinates[12].Y) * y_dis;

            this.AOs = Math.Abs((float)this.mouseCoordinates[17].Y - (float)this.mouseCoordinates[16].Y) * y_dis;
            this.LAs = Math.Abs((float)this.mouseCoordinates[18].Y - (float)this.mouseCoordinates[17].Y) * y_dis;

            this.LA_AO = Math.Abs(this.LAs/this.AOs );
            this.EDV = Math.PI * Math.Pow(this.LVIDd, 3) / 6;
            this.LVmass = 0.8 * Math.Pow((this.IVSd + this.LVIDd + this.LVPWd), 3) - Math.Pow(this.LVIDd, 3);
            this.IVSthickening = Math.Abs(this.IVSs - this.IVSd) / this.IVSd * 100;
            this.FS = Math.Abs(this.LVIDd - this.LVIDs) / this.LVIDd * 100;
            this.ESV = Math.PI * Math.Pow(this.LVIDs, 3) / 6;
            this.LVSV = this.EDV - this.ESV;
            this.LVEF = Math.Abs(this.EDV - this.ESV) / this.EDV * 100;
            this.LVPWthickening = Math.Abs(this.LVPWd - this.LVPWs) / this.LVPWd * 100;
        }

        private void LA_AOMode()
        {

            double y_dis = Math.Abs(4 / (float)(this.y_dist[0].Y - this.y_dist[1].Y));
            
            this.AOs = Math.Abs((float)this.mouseCoordinates[1].Y - (float)this.mouseCoordinates[0].Y) * y_dis;
            this.LAs = Math.Abs((float)this.mouseCoordinates[2].Y - (float)this.mouseCoordinates[1].Y) * y_dis;
            this.LA_AO = Math.Abs(this.LAs/this.AOs);
        }

        private void TwoDim_LV_Volume()
        {
            double y_dis = (float)(Math.Sqrt(Math.Pow(this.y_dist[0].Y - this.y_dist[1].Y, 2) + Math.Pow(this.y_dist[0].X - this.y_dist[1].X, 2)));
            double factor = Math.Abs(3 / y_dis);

            double areaOfRegionCm;

            //Area calculations
            areaOfRegionCm = MainForm.areaInPixels[0] * Math.Pow(factor, 2);
            this.twoDimInfo.EDV = 0.85 * Math.Pow(areaOfRegionCm, 2) / (MainForm.lengthsInPixels[0] * factor);

            areaOfRegionCm = MainForm.areaInPixels[2] * Math.Pow(factor, 2);
            this.twoDimInfo.ESV = 0.85 * Math.Pow(areaOfRegionCm, 2) / (MainForm.lengthsInPixels[2] * factor);

            this.twoDimInfo.SV = Math.Abs(this.twoDimInfo.EDV - this.twoDimInfo.ESV);

            //Mass calculations
            areaOfRegionCm = MainForm.areaInPixels[1] * Math.Pow(factor, 2);
            double mass1 = 0.85 * Math.Pow(areaOfRegionCm, 2) / (MainForm.lengthsInPixels[1] * factor);

            areaOfRegionCm = MainForm.areaInPixels[3] * Math.Pow(factor, 2);
            double mass2 = 0.85 * Math.Pow(areaOfRegionCm, 2) / (MainForm.lengthsInPixels[3] * factor);

            this.twoDimInfo.LVMass = Math.Abs(mass1 - mass2);

            this.twoDimInfo.LVMassPerKg = this.twoDimInfo.LVMass / MainForm.weight;
        }

        private void AorticBloodFlow()
        {
            double noOfPixelsBetwStartAndEnd = (float)(Math.Sqrt(Math.Pow(this.aorticInfo.distStarttoEndClick[0].Y - this.aorticInfo.distStarttoEndClick[1].Y, 2) + 
                Math.Pow(this.aorticInfo.distStarttoEndClick[0].X - this.aorticInfo.distStarttoEndClick[1].X, 2)));

            double avgVel = Math.Abs(MainForm.areaInPixels[0] / ((this.aorticInfo.velocity[0].Y - this.aorticInfo.velocity[1].Y) * 40));

            double TVI = Math.Abs(avgVel * noOfPixelsBetwStartAndEnd / (this.aorticInfo.time[0].X - this.aorticInfo.time[1].X));

            double y_dis = (float)(Math.Sqrt(Math.Pow(this.aorticInfo.diameter[0].Y - this.aorticInfo.diameter[1].Y, 2) + Math.Pow(this.aorticInfo.diameter[0].X - this.aorticInfo.diameter[1].X, 2)));
            double factor = Math.Abs(3 / y_dis);

            this.aorticInfo.CSA = Math.PI * Math.Pow(Math.Abs(this.aorticInfo.diameter[0].Y - this.aorticInfo.diameter[1].Y), 2) / 4 * factor * factor;

            this.aorticInfo.SV = TVI * this.aorticInfo.CSA;

            double HR = Math.Abs((60.0 / ((float)this.aorticInfo.HR[0].X - (float)this.aorticInfo.HR[1].X)) * (float)(this.aorticInfo.time[0].X - this.aorticInfo.time[1].X));

            this.aorticInfo.bloodFlow = this.aorticInfo.SV * HR;
        }

        private void PulmonaryBloodFlow()
        {
            double noOfPixelsBetwStartAndEnd = (float)(Math.Sqrt(Math.Pow(this.pulmonaryInfo.distStarttoEndClick[0].Y - this.pulmonaryInfo.distStarttoEndClick[1].Y, 2) +
                Math.Pow(this.pulmonaryInfo.distStarttoEndClick[0].X - this.pulmonaryInfo.distStarttoEndClick[1].X, 2)));

            double noOfPixelsInOneSecond = this.pulmonaryInfo.time[0].X - this.pulmonaryInfo.time[1].X;

            double noOfPixelsBetwStartAndEndConvertedToSec = noOfPixelsBetwStartAndEnd / noOfPixelsInOneSecond;

            double temp1 = MainForm.areaInPixels[0] / noOfPixelsBetwStartAndEnd;

            double noOfPixelsInVelocity = this.pulmonaryInfo.velocity[0].Y - this.pulmonaryInfo.velocity[1].Y;

            double avgVel = 60 * temp1 / noOfPixelsInVelocity;

           // double avgVel = temp2 / noOfPixelsBetwStartAndEndConvertedToSec;//Math.Abs(MainForm.areaInPixels[0] / ((this.pulmonaryInfo.velocity[0].Y - this.pulmonaryInfo.velocity[1].Y) / 60.0));

            double TVI = Math.Abs(avgVel * noOfPixelsBetwStartAndEnd / (this.pulmonaryInfo.time[0].X - this.pulmonaryInfo.time[1].X));

            double y_dis = (float)(Math.Sqrt(Math.Pow(this.pulmonaryInfo.distance[0].Y - this.pulmonaryInfo.distance[1].Y, 2) + Math.Pow(this.pulmonaryInfo.distance[0].X - this.pulmonaryInfo.distance[1].X, 2)));
            
            double factor = Math.Abs(3 / y_dis);

            double diameter = (float)(Math.Sqrt(Math.Pow((this.pulmonaryInfo.diameter[0].Y - this.pulmonaryInfo.diameter[1].Y) * factor, 2) + Math.Pow((this.pulmonaryInfo.diameter[0].X - this.pulmonaryInfo.diameter[1].X) * factor, 2)));

            this.pulmonaryInfo.CSA = Math.PI * Math.Pow(diameter, 2) / 4;

            this.pulmonaryInfo.SV = TVI * this.pulmonaryInfo.CSA;

            double HR = Math.Abs((60.0 / ((float)this.pulmonaryInfo.HR[0].X - (float)this.pulmonaryInfo.HR[1].X)) * (float)(this.pulmonaryInfo.time[0].X - this.pulmonaryInfo.time[1].X));

            this.pulmonaryInfo.bloodFlow = this.pulmonaryInfo.SV * HR;
        }

        private void SVCBloodFlow()
        {
            double noOfPixelsBetwStartAndEnd = (float)(Math.Sqrt(Math.Pow(this.svcInfo.distStarttoEndClick[0].Y - this.svcInfo.distStarttoEndClick[1].Y, 2) +
                Math.Pow(this.svcInfo.distStarttoEndClick[0].X - this.svcInfo.distStarttoEndClick[1].X, 2)));

            double avgVel = Math.Abs(MainForm.areaInPixels[0] / ((this.svcInfo.velocity[0].Y - this.svcInfo.velocity[1].Y) * 40));

            double TVI = Math.Abs(avgVel * noOfPixelsBetwStartAndEnd / (this.svcInfo.time[0].X - this.svcInfo.time[1].X));

            double y_dis = (float)(Math.Sqrt(Math.Pow(this.svcInfo.diameter[0].Y - this.svcInfo.diameter[1].Y, 2) + Math.Pow(this.svcInfo.diameter[0].X - this.svcInfo.diameter[1].X, 2)));
            double factor = Math.Abs(3 / y_dis);

            this.svcInfo.CSA = Math.PI * Math.Pow(Math.Abs(this.svcInfo.diameter[0].Y - this.svcInfo.diameter[1].Y), 2) / 4 * factor * factor;

            this.svcInfo.SV = TVI * this.svcInfo.CSA;

            double HR = Math.Abs((60.0 / ((float)this.svcInfo.HR[0].X - (float)this.svcInfo.HR[1].X)) * (float)(this.svcInfo.time[0].X - this.svcInfo.time[1].X));

            this.svcInfo.bloodFlow = this.svcInfo.SV * HR;
        }

        private void PDADiameter()
        {
            double y_dis = Math.Abs(3 / (float)(this.y_dist[0].Y - this.y_dist[1].Y));
            this.pdaInfo.PDA_diameter = Math.Abs((float)this.pdaInfo.diameter[0].Y - (float)this.pdaInfo.diameter[1].Y) * y_dis * 10;
        }

        private void RVPressure()
        {
            double y_dis = Math.Abs(3 / (float)(this.y_dist[0].Y - this.y_dist[1].Y));
            double l = ((float)this.rvInfo.pressure_tr_jet[0].Y - (float)this.rvInfo.pressure_tr_jet[1].Y) * y_dis * 10;
            this.rvInfo.RV_pressure = Math.Abs(l * l * 4 + 10);
        }
    }

    public class TwoDimInfo
    {
        public double EDV;
        public double ESV;
        public double LVMass;
        public double LVMassPerKg;
        public double SV;

        public TwoDimInfo()
        {
            this.EDV = this.ESV = this.LVMass = this.LVMassPerKg = 0.0;
        }
    }

    public class AorticInfo
    {
        public Point[] velocity;
        public Point[] time;
        public Point[] HR;
        public Point[] distance;
        public Point[] diameter;
        public Point[] distStarttoEndClick;
        public double CSA;
        public double SV;
        public double bloodFlow;
        public int areaOfVelContourInPixels;

        public AorticInfo()
        {
            this.velocity = new Point[2];
            this.time = new Point[2];
            this.HR = new Point[2];
            this.distance = new Point[2];
            this.diameter = new Point[2];
            this.distStarttoEndClick = new Point[2];
            this.areaOfVelContourInPixels = 0;
        }
    }

    public class PulmonaryInfo
    {
        public Point[] velocity;
        public Point[] time;
        public Point[] HR;
        public Point[] distance;
        public Point[] diameter;
        public Point[] distStarttoEndClick;
        public double CSA;
        public double SV;
        public double bloodFlow;
        public int areaOfVelContourInPixels;

        public PulmonaryInfo()
        {
            this.velocity = new Point[2];
            this.time = new Point[2];
            this.HR = new Point[2];
            this.distance = new Point[2];
            this.diameter = new Point[2];
            this.distStarttoEndClick = new Point[2];
            this.areaOfVelContourInPixels = 0;
        }
    }

    public class SVCInfo
    {
        public Point[] velocity;
        public Point[] time;
        public Point[] HR;
        public Point[] distance;
        public Point[] diameter;
        public Point[] distStarttoEndClick;
        public double CSA;
        public double SV;
        public double bloodFlow;
        public int areaOfVelContourInPixels;

        public SVCInfo()
        {
            this.velocity = new Point[2];
            this.time = new Point[2];
            this.HR = new Point[2];
            this.distance = new Point[2];
            this.diameter = new Point[2];
            this.distStarttoEndClick = new Point[2];
            this.areaOfVelContourInPixels = 0;
        }
    }

    public class PDAInfo
    {        
        public Point[]  diameter;
        public double PDA_diameter;

        public PDAInfo()
        {
            this.diameter = new Point[2];
            this.PDA_diameter = 0.0;
        }
    }

    public class RVPressureInfo
    {
        public Point[] pressure_tr_jet;
        public double RV_pressure;

        public RVPressureInfo()
        {
            this.pressure_tr_jet = new Point[2];
            this.RV_pressure = 0.0;
        }
    }

    public enum CalculationModes
    {
        LV,
        LA_AO,
        TwoDim_LV_Volume,
        Aortic_Blood_Flow,
        Pulmonary_Blood_Flow,
        SVC_Blood_Flow,
        PDA,
        RVPressure
    };
}
