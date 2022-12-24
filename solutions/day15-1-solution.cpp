#include<iostream>
#include<vector>
#include<set>
#include<utility>
#include<fstream>
#include<string>
#include<numeric>
#include<algorithm>

#include"../include/my_utils.h"

/* ------- */
/* CLASSES */
/* ------- */

typedef std::pair<int,int> Coord;

class Sensor {
    public:
        Coord location;
        Coord closest_beacon;
        /* Range is the manhattan distance from the 
         * sensor's location to it's closest beacon. */
        int range;

        Sensor(std::string line) {
            parse_from_line(line);
            range =
                std::abs(location.first - closest_beacon.first) +
                std::abs(location.second - closest_beacon.second);
        }

        void parse_from_line(std::string line) {
                auto first_x_coord_pos = line.find("x=") + 2;
                auto first_comma_pos = line.find(",");
                auto first_y_coord_pos = line.find("y=") + 2;
                auto colon_pos = line.find(":");
        
                auto first_x = line.substr(
                    first_x_coord_pos,
                    first_comma_pos - first_x_coord_pos
                );
                auto first_y = line.substr(
                    first_y_coord_pos,
                    colon_pos - first_y_coord_pos
                );

                auto last_x_coord_pos = line.find("x=", colon_pos) + 2;
                auto last_comma_pos = line.find(",", colon_pos);
                auto last_y_coord_pos = line.find("y=", colon_pos) + 2;
               
                auto second_x = line.substr(
                    last_x_coord_pos,
                    last_comma_pos - last_x_coord_pos
                );
                auto second_y = line.substr(
                    last_y_coord_pos
                );

                this->location = std::make_pair(
                    std::stoi(first_x, nullptr, 10),
                    std::stoi(first_y, nullptr, 10)
                );
                this->closest_beacon = std::make_pair(
                    std::stoi(second_x, nullptr, 10),
                    std::stoi(second_y, nullptr, 10)
                );
        }

        bool can_sense(Coord coord) {
            int range_to_coord =
                std::abs(location.first - coord.first) +
                std::abs(location.second - coord.second);
            return range_to_coord <= range;
        }

    private:
        friend std::ostream& operator<<(std::ostream& out, Sensor& sensor);
};

std::ostream& operator<<(std::ostream& out, Sensor& sensor) {
    out<<"Sensor("<<"location="<<sensor.location;
    return out<<", closest_beacon="<<sensor.closest_beacon<<", range="<<sensor.range<<")";
}

/* --------------- */
/* COORD FUNCTIONS */
/* --------------- */

Coord get_coord_after_coord(Coord coord) {
    return std::make_pair(coord.first+1, coord.second);
}

bool coord_out_of_bounds(Coord coord, int leftmost, int rightmost) {
    return ((coord.first < leftmost) || (coord.first > rightmost));
}

int get_distance(Coord c1, Coord c2) {
    return std::abs(c1.first - c2.first) + std::abs(c1.second - c2.second);
}

/* ---------------- */
/* SENSOR FUNCTIONS */
/* ---------------- */

auto get_sensor_within_range(
    std::vector<Sensor>& sensors,
    Coord coord
) {
    return std::find_if(
        sensors.begin(),
        sensors.end(),
        [coord] (Sensor& sensor) -> bool {
            return sensor.can_sense(coord);
        }
    );
}

int jumps_needed_to_escape_sensor_range(
    Coord curr_coord,
    Sensor sensor,
    int leftmost_x_coord,
    int rightmost_x_coord
) {
    Coord next_coord = get_coord_after_coord(curr_coord);

    /* If next coord is out of bounds, only one jump needed to escape. */
    if (coord_out_of_bounds(next_coord, leftmost_x_coord, rightmost_x_coord)) return 1; 

    int sensor_to_curr_coord = get_distance(curr_coord, sensor.location);
    int sensor_to_next_coord = get_distance(next_coord, sensor.location);
    
    bool sensor_can_sense_next_coord =
        sensor_to_next_coord <= sensor.range;
    bool sensor_range_getting_stronger =
        (sensor_to_curr_coord - sensor_to_next_coord) > 0;

    if (sensor_can_sense_next_coord && sensor_range_getting_stronger) {
        int distance_to_sensor_x_coord =
            sensor.location.first - curr_coord.first;
        return distance_to_sensor_x_coord*2;
    }
    else if(sensor_can_sense_next_coord && !sensor_range_getting_stronger) {
        return (sensor.range - sensor_to_curr_coord);
    }

    /* If sensor can't sense the next coord... */
    return 1;
}

int count_impossible_beacon_locations_at_row(
    int row_to_check,
    int leftmost_x_coord,
    int rightmost_x_coord,
    std::vector<Sensor>& sensors,
    std::set<Coord>& beacons
) {
    int count = 0;

    for (int x_coord = leftmost_x_coord; x_coord <= rightmost_x_coord;) {
        auto coord = std::make_pair(x_coord, row_to_check);

        bool out_of_bounds = coord_out_of_bounds(
            coord,
            leftmost_x_coord,
            rightmost_x_coord
        );
        if (out_of_bounds) break;

        /* If beacon is encountered... */
        if (beacons.find(coord) != beacons.end()) {
            x_coord++;
            continue;
        }

        auto sensor_within_range = get_sensor_within_range(sensors, coord);
        
        /* If coord is traceable by sensor... */
        if (sensor_within_range != sensors.end()) {
            auto jumps = jumps_needed_to_escape_sensor_range(
                coord,
                *sensor_within_range,
                leftmost_x_coord,
                rightmost_x_coord
            );

            x_coord += jumps;
            count += jumps;
        }
        /* If coord isn't traceable, we move to the next one. */
        else {
            x_coord++;
            continue;
        }
    }

    return count;
}

/* ---- */
/* MAIN */
/* ---- */

int main() {
    std::ifstream file;
    file.open("input/day15-input.txt", std::ios::in);

    std::vector<Sensor> sensors;
    std::set<Coord> beacons;

    /* We parse the input. */
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (file.eof()) break;
        if (line.length() == 0) break;

        sensors.push_back(Sensor(line));
    }

    /* We get all the beacons. */
    std::for_each(
        sensors.begin(),
        sensors.end(),
        [&beacons] (Sensor& sensor) -> void {
            beacons.insert(sensor.closest_beacon);
        }
    );

    /* We get the left and rightmost coordinates to iterate over. */
    auto largest_range = std::reduce(
        sensors.begin(),
        sensors.end(),
        0,
        [] (int previous_max, Sensor s1) -> int {
            return (s1.range > previous_max)? s1.range : previous_max;
        }
    );
    auto leftmost_x_coordinate = std::reduce(
        sensors.begin(),
        sensors.end(),
        sensors.at(0).location.first,
        [] (int previous_min, Sensor s1) -> int {
            previous_min = (s1.location.first < previous_min)?
                s1.location.first : previous_min;
            previous_min = (s1.closest_beacon.first < previous_min)?
                s1.closest_beacon.first : previous_min;
            return previous_min;
        }
    ) - largest_range - 1;
    auto rightmost_x_coordinate = std::reduce(
        sensors.begin(),
        sensors.end(),
        sensors.at(0).location.first,
        [] (int previous_max, Sensor s1) -> int {
            previous_max = (s1.location.first > previous_max)?
                s1.location.first : previous_max;
            previous_max = (s1.closest_beacon.first > previous_max)?
                s1.closest_beacon.first : previous_max;
            return previous_max;
        }
    ) + largest_range + 1;
    int row_to_check = 2000000;

    int impossible_beacon_locations = count_impossible_beacon_locations_at_row(
        row_to_check,
        leftmost_x_coordinate,
        rightmost_x_coordinate,
        sensors,
        beacons
    );

    std::cout<<impossible_beacon_locations<<std::endl;

    file.close();
    return 0;
}
