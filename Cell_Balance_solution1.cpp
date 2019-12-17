#include <iostream>
#include <bits/stdc++.h>
#include <string>
///using namespace std<br>
using namespace std;
///Maximum Battery Voltage (Assumption).<br>
#define max_batt_vol 15
///Node Voltage To Be Maintained in Volts (Assumption).<br> 
#define nodeVoltage 8.0 
///Cutoff Voltage For a Cell in Volts.<br> 
#define min_limit 8.0
///Initial Cell 1 Voltage.<br>    
#define b1 14.1 
///Initial Cell 2 Voltage.<br>         
#define b2 12.2 
///Initial Cell 3 Voltage.<br>         
#define b3 10.3 
///Load Resistance in ohms.<br>        
#define R_load 10.0 
///Branch Resistance 1.<br>   
#define R_1 1.0 
///Branch Resistance 2.<br>        
#define R_2 2.0
///Branch Resistance 3.<br>         
#define R_3 3.0 
///Stepping time in hours (Assumption).<br>        
#define dt 0.01
///Batary Capacity in Ampere hours (Assumption). <br>        
#define bat_cap 0.5
///Simulation Time in Hours (Assumption).<br>     
#define sim_time 10     

bool sortbyforth(const tuple<float, float, float, float, string, float,float> &a,
                 const tuple<float, float, float, float, string, float,float> &b)
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
    cout << "Branch Resistances are respectively " << R_1 << ", " << R_2 << ", " << R_3 << " Ohms\n";
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
	///<b> Initialized Time Remaining with Simulation Time</b><br>
	///Initiliaze float time_rem = sim_time;<br>
    float time_rem = sim_time;
	///<b> Initializing Duty Cycles & current With Zero.</b><br>
    float d1 = 0;
    float d2 = 0;
    float d3 = 0;
	float current = 0;
	///<b> Defining Vector.</b><br>
	///vector<tuple<float, float, float, float, string, float,float>>bat;<br>
    vector<tuple<float, float, float, float, string, float, float>> bat;
	/// <b>Inserting Branch Parameters in Vector Tuple.</b><br>
	
    bat.push_back(make_tuple(b1, R_1, d1, 1, "OFF", bat_cap,current));
    bat.push_back(make_tuple(b2, R_2, d2, 2, "OFF", bat_cap,current));
    bat.push_back(make_tuple(b3, R_3, d3, 3, "OFF", bat_cap,current));

	///<b> To Display Initial State of Switches</b><br>
	///cout << "Sr No.\tBat Vol\tState\tCurrent\n";<br>
    cout << "Sr_No.\tBat_Vol\tState\tCurrent\n";
	
    for (int i = 0; i < bat.size(); i++)
    {
        cout << get<3>(bat[i]) << "\t"
                 << get<0>(bat[i]) << "\t"
                 << get<4>(bat[i]) << "\t"
				 <<get<6>(bat[i]) << "\n";
    }
	///<b> ==========================================================.</b><br>
    cout << "===========================\n";
	///cout << endl;<br>
    cout << endl;
	///<b> Algorithm Starts...................</b><br>
    cout << "Algorithm Starts...........\n\n";
	
	///<b>while (time_rem > 0)</b><br>
 ///{<br>
    while (time_rem > 0)
    {
		///<b> Sorting With Respect To Cell Voltage.</b><br>
		/// sort(bat.begin(), bat.end());<br>
		
        sort(bat.begin(), bat.end());
		///<b> Assigning V_max= Highest Voltage.</b><br>
		/// float V_max = get<0>(bat[2]);<br>
		
        float V_max = get<0>(bat[2]);
		///<b> Assigning V_mid= Middle Voltage</b><br>.
		/// float V_mid = get<0>(bat[1]);<br>
		
        float V_mid = get<0>(bat[1]);
		///<b> Assigning V_low= Lowest Voltage.</b><br>
		/// float V_low = get<0>(bat[0]);<br>
		
        float V_low = get<0>(bat[0]);
		///<b> Assigning R_max= Branch resistance of Highest Voltage.</b><br>
		/// float R_max = get<1>(bat[2]);<br>
		
        float R_max = get<1>(bat[2]);
		///<b> Assigning Capacity= Capacity of Highest Voltage cell.</b><br>
		/// float capacity = get<5>(bat[2]);<br>
		
        float capacity = get<5>(bat[2]);
		///<b>Assigning ON State to Highest Voltage Cell and Keeping other two OFF.</b><br>
        get<4>(bat[2]) = "ON";
        get<4>(bat[1]) = "OFF";
        get<4>(bat[0]) = "OFF";
	
        if (V_max <= min_limit || V_mid <= min_limit || V_low <= min_limit)
        {
            break;
        }
		
		///<b> finding Duty Cycle.</b><br>
        float d_max = (nodeVoltage * R_max) / (R_load * (V_max - nodeVoltage));
        if (d_max > 1)
        {
            d_max = 1;
        }
		///<b> Calculating Load current.</b><br>
		/// float curr = nodeVoltage / R_load;<br>
        float curr = nodeVoltage / R_load;
		
		
        //cout<<curr<<endl;
        float node_vol = (V_max - curr * (R_max / d_max));
        //cout << "R_internal: " << R_max / d_max << endl;<br>
        float currr=node_vol/R_load;
        get<6>(bat[2])=currr;
		get<6>(bat[1])=0;
		get<6>(bat[0])=0;

        //float cap_rem = bat_cap - (curr * dt) / bat_cap;<br>
		 capacity = capacity - d_max*curr*dt;
        //cout << capacity << endl;
	  float percent_cap = (capacity / bat_cap) * 100;
        V_max = max_batt_vol * ((percent_cap * 0.6 / 100) + 0.4); //Linear relationship

		///<b> Assigning New Cell Voltage.</b><br>
		///get<0>(bat[2]) = V_max;<br>
		 get<0>(bat[2]) = V_max;
		get<2>(bat[2]) = d_max;
		 get<5>(bat[2]) = capacity;
        sort(bat.begin(), bat.end(), sortbyforth);

		///<b> Displaying Serial Number, Battery Voltage and, Switch State of Each Branch.</b><br>
        for (int i = 0; i < bat.size(); i++)
        {
            cout << get<3>(bat[i]) << "\t"
                 << get<0>(bat[i]) << "\t"
                 << get<4>(bat[i]) << "\t"
				 <<get<6>(bat[i]) << "\n";
        }
		///<b> Display Node_Vol After Each Step time</b> <br>
        cout << "System Voltage: " << node_vol << endl;
        cout << endl;
        time_rem = time_rem - dt;
    }
    return 0;
}  

