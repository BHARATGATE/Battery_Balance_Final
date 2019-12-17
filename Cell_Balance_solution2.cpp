#include <iostream>
#include <bits/stdc++.h>
#include <string>
///using namespace std<br>
using namespace std;
#define max_batt_vol 15 //max battery voltage (Assumption) 
#define nodeVoltage 8.0 //Node voltage to maintain in Volts (Assumption)
#define min_limit 8.0   //Cutoff voltage for a cell in Volts
#define b1 14.3         //initial cell voltage 1
#define b2 13.2         //initial cell voltage 2
#define b3 11.1         //initial cell voltage 3 
#define R_load 10.0     //in ohms
#define dt 0.01         //stepping time in hours (Assumption)  
#define bat_cap 2.5     //in ampere hours at 15 V (Assumption) 
#define sim_time 10     //in hours (Assumption)

bool sortbyforth(const tuple<float, float, float, float, string, float, float> &a,
                 const tuple<float, float, float, float, string, float, float> &b)
{
    return (get<3>(a) < get<3>(b));
}

int main()
{
    cout << "===========================\n";
    cout << "Cell Balancing Algorithm\n";
    cout << "---------------------------\n";
    cout << "Node Voltage will be maintained at: " << nodeVoltage << " Volts\n";
    cout << "Each cell can discharge upto: " << min_limit << " Volts\n";
    cout << "Load Resistance is: " << R_load << " Ohms\n";
    cout << "Initial Cell Voltages are respectively " << b1 << ", " << b2 << ", " << b3 << " Volts\n";
    cout << "---------------------------\n";
    cout << "Assumptions\n";
    cout << "---------------------------\n";
    cout << "Battery Capacity is: " << bat_cap << " Ah\n";
    cout << "Simulation time is: " << sim_time << " Hours\n";
    cout << "Stepping time in simulation is: " << dt << " Hours\n";
    cout << "Linear relationship between state of charge and voltage\n";
    cout << "---------------------------\n";
	
	///<b> Initialized Time Remaining  with Simulation Time</b><br>
	///Initiliaze float time_rem = sim_time;<br>

    float time_rem = sim_time;
    float d1 = 0;
    float d2 = 0;
    float d3 = 0;
	float current = 0;
	///<b> Since Two Branches Sharing Equal Current</b><br>
	///float curr = nodeVoltage / (2*R_load);<br>
    float curr = nodeVoltage / (2*R_load);
	///<b> Calculating Branch Resistance to Omit Circulating Current</b><br>
	float R_1=((b1-8)/curr);
	float R_2=((b2-8)/curr);
	float R_3=((b3-8)/curr);
	// Calculating each cell capacity 
	float bat_cap1=b1*2.5/15;
	float bat_cap2=b2*2.5/15;
	float bat_cap3=b3*2.5/15;
	///<b> Defining Vector.</b><br>
	///vector<tuple<float, float, float, float, string, float, float>>bat;<br>
	vector<tuple<float, float, float, float, string, float, float>> bat;
	/// <b>Inserting Branch Parameters in Vector Tuple.</b><br>
    bat.push_back(make_tuple(b1, R_1, d1, 1, "OFF", bat_cap,current));
    bat.push_back(make_tuple(b2, R_2, d2, 2, "OFF", bat_cap,current));
    bat.push_back(make_tuple(b3, R_3, d3, 3, "OFF", bat_cap,current));
    float max_voltage=get<0>(bat[0]);
	///<b> Finding maximum voltage for the linear curve</b><br>
	///Let float max_voltage=get<0>(bat[0]);<br>
	// Finding maximum voltage for the linear curve
    for(int i=1;i<3;i++){
    	if(get<0>(bat[i])>max_voltage){
    		max_voltage=get<0>(bat[i]);
    	}
    }
///<b> To Display Initial State of Switches</b><br>
    cout << "Sr No.\tBat Vol\tState\tCurrent\n";
    for (int i = 0; i < bat.size(); i++)
    {
        cout << get<3>(bat[i]) << "\t"
             << get<0>(bat[i]) << "\t"
             << get<4>(bat[i]) << "\t"
			 <<get<6>(bat[i]) << "\n";
    }
	///<b> ==========================================================.</b><br>
    cout << "===========================\n";
    cout << endl;
    cout << "Algorithm Starts...........\n\n";
	while (time_rem > 0)
    {
		
		///<b> Sorting With Respect To Cell Voltage.</b><br>
        sort(bat.begin(), bat.end());
		///<b> Assigning V_max= Highest Voltage.</b><br>
        float V_max = get<0>(bat[2]);
		///<b> Assigning V_mid= Middle Voltage</b><br>.
        float V_mid = get<0>(bat[1]);
		///<b> Assigning V_low= Lowest Voltage.</b><br>
        float V_low = get<0>(bat[0]);
		///<b> Assigning R_max= Branch resistance of Highest Voltage.</b><br>
		 float R_max = get<1>(bat[2]);
		 ///<b> Assigning R_mid= Branch Resistance of Medium  Voltage.</b><br>
		float R_mid = get<1>(bat[1]);
		///<b>Assigning ON State to Highest Voltage Cell & Medium Voltage Cell and Keeping other One OFF.</b><br>
        get<4>(bat[2]) = "ON";
        get<4>(bat[1]) = "ON";
        get<4>(bat[0]) = "OFF";
		///<b> If Any Cell Voltage < 8 V, then Simulation Stops.</b><br>
        if (V_max <= min_limit || V_mid <= min_limit || V_low <= min_limit)
        {
            break;
        }
		///<b> finding Duty Cycle.</b><br>
		//float d_max=0.1;
        float d_max = (nodeVoltage * R_max) / (R_load * (V_max - nodeVoltage));
        if (d_max > 1)
        {
            d_max = 1;
        }
        float d_mid = (nodeVoltage * R_mid) / (R_load * (V_mid - nodeVoltage));
        if (d_mid > 1)
        {
            d_mid = 1;
        }
		///<b>	Effective Resistance= R_max / d_max.</b><br>
		///<b> Calculating Node Voltage Based on Effective Resistance.</b><br>
        float node_vol_1 = (V_max - curr * (R_max /d_max));
		float node_vol_2 = (V_mid - curr * (R_mid /d_mid ));
		get<6>(bat[2])=curr;
		get<6>(bat[1])=curr;
		get<6>(bat[0])=0;

        //float cap_rem = bat_cap - (curr * dt) / bat_cap;
        //capacity = capacity - (capacity * dt) / (curr);
		///<b> Remaining capacity of Highest Cell Voltage & Medium Cell Voltage After Each Simulation Step.</b><br>
        get<5>(bat[2]) = get<5>(bat[2]) - dt * curr * d_max;
		get<5>(bat[1]) = get<5>(bat[1]) - dt * curr * d_max;
        //cout << capacity << endl;
		///<b> Calculating Percent Capacity.</b><br>
        float percent_cap_1 = (get<5>(bat[2]) / bat_cap) * 100;
		float percent_cap_2 = (get<5>(bat[1]) / bat_cap) * 100;
		///<b>cout << " Percent Capacity of ON switches: "<<percent_cap_1 << " "<<percent_cap_2 << endl;</b><br>
		//cout << " Percent Capacity of ON switches: "<<percent_cap_1 << " "<<percent_cap_2 << endl;
		//<b> New Cell Voltage of Highest Cell Voltage and Medium Cell Volatge After Each Simulation Step.</b><br>
        V_max = max_voltage * ((percent_cap_1 * 0.6 / 100) + 0.4); //Linear relationship
		V_mid = max_voltage * ((percent_cap_2 * 0.6 / 100) + 0.4); //Linear relationship

        //V_max = percent_cap * V_max;
///<b> Assigning New Cell Voltage.</b><br>
		///get<0>(bat[2]) = V_max;<br>
        get<0>(bat[2]) = V_max;
		///<b> Assigning New Cell Voltage.</b><br>
		///get<0>(bat[2]) = V_mid;<br>
		get<0>(bat[1]) = V_mid;
		///<b> Assigning New Value of Duty Cycle.</b><br>
		///get<2>(bat[2]) = d_max;<br>
        get<2>(bat[2]) = d_max;
		///<b> Assigning New Value of Duty Cycle.</b><br>
		///get<2>(bat[1]) = d_mid;<br>
		get<2>(bat[1]) = d_max;
		///<b> Sorting Accoring to Serial Number.</b><br>
		///sort(bat.begin(), bat.end(), sortbyforth);<br>
		///<b> Displaying Serial Number, Battery Voltage and, Switch State of Each Branch.</b><br>
        sort(bat.begin(), bat.end(), sortbyforth);

        for (int i = 0; i < bat.size(); i++)
        {
            cout << get<3>(bat[i]) << "\t"
                 << get<0>(bat[i]) << "\t"
                 << get<4>(bat[i]) << "\t"
				 <<get<6>(bat[i]) << "\n";
        }
        cout << "System Voltage: " << node_vol_1 <<"\t" << node_vol_2  << endl;
        //cout << endl;
		 time_rem = time_rem - dt;
    }
    return 0;
}
