/**************************************************************************/
/*  test_navigation_server_3d.h                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_NAVIGATION_SERVER_3D_H
#define TEST_NAVIGATION_SERVER_3D_H

#include "servers/navigation_server_3d.h"

#include "tests/test_macros.h"

namespace TestNavigationServer3D {
TEST_SUITE("[Navigation]") {
	TEST_CASE("[NavigationServer3D] Server should be empty when initialized") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();
		CHECK_EQ(navigation_server->get_maps().size(), 0);

		SUBCASE("'ProcessInfo' should report all counters empty as well") {
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_ACTIVE_MAPS), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_REGION_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_AGENT_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_LINK_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_POLYGON_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_EDGE_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_EDGE_MERGE_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_EDGE_CONNECTION_COUNT), 0);
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_EDGE_FREE_COUNT), 0);
		}
	}

	TEST_CASE("[NavigationServer3D] Server should manage agent properly") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();

		RID agent = navigation_server->agent_create();
		CHECK(agent.is_valid());

		SUBCASE("'ProcessInfo' should not report dangling agent") {
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_AGENT_COUNT), 0);
		}

		SUBCASE("Setters/getters should work") {
			bool initial_use_3d_avoidance = navigation_server->agent_get_use_3d_avoidance(agent);
			navigation_server->agent_set_use_3d_avoidance(agent, !initial_use_3d_avoidance);
			navigation_server->process(0.0); // Give server some cycles to commit.

			CHECK_EQ(navigation_server->agent_get_use_3d_avoidance(agent), !initial_use_3d_avoidance);
			// TODO: Add remaining setters/getters once the missing getters are added.
		}

		SUBCASE("'ProcessInfo' should report agent with active map") {
			RID map = navigation_server->map_create();
			CHECK(map.is_valid());
			navigation_server->map_set_active(map, true);
			navigation_server->agent_set_map(agent, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_AGENT_COUNT), 1);
			navigation_server->agent_set_map(agent, RID());
			navigation_server->free(map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_AGENT_COUNT), 0);
		}

		navigation_server->free(agent);
	}

	TEST_CASE("[NavigationServer3D] Server should manage map properly") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();

		RID map;
		CHECK_FALSE(map.is_valid());

		SUBCASE("Queries against invalid map should return empty or invalid values") {
			CHECK_EQ(navigation_server->map_get_closest_point(map, Vector3(7, 7, 7)), Vector3());
			CHECK_EQ(navigation_server->map_get_closest_point_normal(map, Vector3(7, 7, 7)), Vector3());
			CHECK_FALSE(navigation_server->map_get_closest_point_owner(map, Vector3(7, 7, 7)).is_valid());
			CHECK_EQ(navigation_server->map_get_closest_point_to_segment(map, Vector3(7, 7, 7), Vector3(8, 8, 8), true), Vector3());
			CHECK_EQ(navigation_server->map_get_closest_point_to_segment(map, Vector3(7, 7, 7), Vector3(8, 8, 8), false), Vector3());
			CHECK_EQ(navigation_server->map_get_path(map, Vector3(7, 7, 7), Vector3(8, 8, 8), true).size(), 0);
			CHECK_EQ(navigation_server->map_get_path(map, Vector3(7, 7, 7), Vector3(8, 8, 8), false).size(), 0);

			Ref<NavigationPathQueryParameters3D> query_parameters = memnew(NavigationPathQueryParameters3D);
			query_parameters->set_map(map);
			query_parameters->set_start_position(Vector3(7, 7, 7));
			query_parameters->set_target_position(Vector3(8, 8, 8));
			Ref<NavigationPathQueryResult3D> query_result = memnew(NavigationPathQueryResult3D);
			navigation_server->query_path(query_parameters, query_result);
			CHECK_EQ(query_result->get_path().size(), 0);
			CHECK_EQ(query_result->get_path_types().size(), 0);
			CHECK_EQ(query_result->get_path_rids().size(), 0);
			CHECK_EQ(query_result->get_path_owner_ids().size(), 0);
		}

		map = navigation_server->map_create();
		CHECK(map.is_valid());
		CHECK_EQ(navigation_server->get_maps().size(), 1);

		SUBCASE("'ProcessInfo' should not report inactive map") {
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_ACTIVE_MAPS), 0);
		}

		SUBCASE("Setters/getters should work") {
			navigation_server->map_set_cell_size(map, 0.55);
			navigation_server->map_set_edge_connection_margin(map, 0.66);
			navigation_server->map_set_link_connection_radius(map, 0.77);
			navigation_server->map_set_up(map, Vector3(1, 0, 0));
			bool initial_use_edge_connections = navigation_server->map_get_use_edge_connections(map);
			navigation_server->map_set_use_edge_connections(map, !initial_use_edge_connections);
			navigation_server->process(0.0); // Give server some cycles to commit.

			CHECK_EQ(navigation_server->map_get_cell_size(map), doctest::Approx(0.55));
			CHECK_EQ(navigation_server->map_get_edge_connection_margin(map), doctest::Approx(0.66));
			CHECK_EQ(navigation_server->map_get_link_connection_radius(map), doctest::Approx(0.77));
			CHECK_EQ(navigation_server->map_get_up(map), Vector3(1, 0, 0));
			CHECK_EQ(navigation_server->map_get_use_edge_connections(map), !initial_use_edge_connections);
		}

		SUBCASE("'ProcessInfo' should report map iff active") {
			navigation_server->map_set_active(map, true);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK(navigation_server->map_is_active(map));
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_ACTIVE_MAPS), 1);
			navigation_server->map_set_active(map, false);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_ACTIVE_MAPS), 0);
		}

		SUBCASE("Number of agents should be reported properly") {
			RID agent = navigation_server->agent_create();
			CHECK(agent.is_valid());
			navigation_server->agent_set_map(agent, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_agents(map).size(), 1);
			navigation_server->free(agent);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_agents(map).size(), 0);
		}

		SUBCASE("Number of links should be reported properly") {
			RID link = navigation_server->link_create();
			CHECK(link.is_valid());
			navigation_server->link_set_map(link, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_links(map).size(), 1);
			navigation_server->free(link);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_links(map).size(), 0);
		}

		SUBCASE("Number of obstacles should be reported properly") {
			RID obstacle = navigation_server->obstacle_create();
			CHECK(obstacle.is_valid());
			navigation_server->obstacle_set_map(obstacle, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_obstacles(map).size(), 1);
			navigation_server->free(obstacle);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_obstacles(map).size(), 0);
		}

		SUBCASE("Number of regions should be reported properly") {
			RID region = navigation_server->region_create();
			CHECK(region.is_valid());
			navigation_server->region_set_map(region, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_regions(map).size(), 1);
			navigation_server->free(region);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->map_get_regions(map).size(), 0);
		}

		SUBCASE("Queries against empty map should return empty or invalid values") {
			navigation_server->map_set_active(map, true);
			navigation_server->process(0.0); // Give server some cycles to commit.

			CHECK_EQ(navigation_server->map_get_closest_point(map, Vector3(7, 7, 7)), Vector3());
			CHECK_EQ(navigation_server->map_get_closest_point_normal(map, Vector3(7, 7, 7)), Vector3());
			CHECK_FALSE(navigation_server->map_get_closest_point_owner(map, Vector3(7, 7, 7)).is_valid());
			CHECK_EQ(navigation_server->map_get_closest_point_to_segment(map, Vector3(7, 7, 7), Vector3(8, 8, 8), true), Vector3());
			CHECK_EQ(navigation_server->map_get_closest_point_to_segment(map, Vector3(7, 7, 7), Vector3(8, 8, 8), false), Vector3());
			CHECK_EQ(navigation_server->map_get_path(map, Vector3(7, 7, 7), Vector3(8, 8, 8), true).size(), 0);
			CHECK_EQ(navigation_server->map_get_path(map, Vector3(7, 7, 7), Vector3(8, 8, 8), false).size(), 0);

			Ref<NavigationPathQueryParameters3D> query_parameters = memnew(NavigationPathQueryParameters3D);
			query_parameters->set_map(map);
			query_parameters->set_start_position(Vector3(7, 7, 7));
			query_parameters->set_target_position(Vector3(8, 8, 8));
			Ref<NavigationPathQueryResult3D> query_result = memnew(NavigationPathQueryResult3D);
			navigation_server->query_path(query_parameters, query_result);
			CHECK_EQ(query_result->get_path().size(), 0);
			CHECK_EQ(query_result->get_path_types().size(), 0);
			CHECK_EQ(query_result->get_path_rids().size(), 0);
			CHECK_EQ(query_result->get_path_owner_ids().size(), 0);

			navigation_server->map_set_active(map, false);
			navigation_server->process(0.0); // Give server some cycles to commit.
		}

		navigation_server->free(map);
		navigation_server->process(0.0); // Give server some cycles to actually remove map.
		CHECK_EQ(navigation_server->get_maps().size(), 0);
	}

	TEST_CASE("[NavigationServer3D] Server should manage link properly") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();

		RID link = navigation_server->link_create();
		CHECK(link.is_valid());

		SUBCASE("'ProcessInfo' should not report dangling link") {
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_LINK_COUNT), 0);
		}

		SUBCASE("Setters/getters should work") {
			bool initial_bidirectional = navigation_server->link_is_bidirectional(link);
			navigation_server->link_set_bidirectional(link, !initial_bidirectional);
			navigation_server->link_set_end_position(link, Vector3(7, 7, 7));
			navigation_server->link_set_enter_cost(link, 0.55);
			navigation_server->link_set_navigation_layers(link, 6);
			navigation_server->link_set_owner_id(link, ObjectID((int64_t)7));
			navigation_server->link_set_start_position(link, Vector3(8, 8, 8));
			navigation_server->link_set_travel_cost(link, 0.66);
			navigation_server->process(0.0); // Give server some cycles to commit.

			CHECK_EQ(navigation_server->link_is_bidirectional(link), !initial_bidirectional);
			CHECK_EQ(navigation_server->link_get_end_position(link), Vector3(7, 7, 7));
			CHECK_EQ(navigation_server->link_get_enter_cost(link), doctest::Approx(0.55));
			CHECK_EQ(navigation_server->link_get_navigation_layers(link), 6);
			CHECK_EQ(navigation_server->link_get_owner_id(link), ObjectID((int64_t)7));
			CHECK_EQ(navigation_server->link_get_start_position(link), Vector3(8, 8, 8));
			CHECK_EQ(navigation_server->link_get_travel_cost(link), doctest::Approx(0.66));
		}

		SUBCASE("'ProcessInfo' should report link with active map") {
			RID map = navigation_server->map_create();
			CHECK(map.is_valid());
			navigation_server->map_set_active(map, true);
			navigation_server->link_set_map(link, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_LINK_COUNT), 1);
			navigation_server->link_set_map(link, RID());
			navigation_server->free(map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_LINK_COUNT), 0);
		}

		navigation_server->free(link);
	}

	TEST_CASE("[NavigationServer3D] Server should manage obstacles properly") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();

		RID obstacle = navigation_server->obstacle_create();
		CHECK(obstacle.is_valid());

		// TODO: Add tests for setters/getters once getters are added.

		navigation_server->free(obstacle);
	}

	TEST_CASE("[NavigationServer3D] Server should manage regions properly") {
		NavigationServer3D *navigation_server = NavigationServer3D::get_singleton();

		RID region = navigation_server->region_create();
		CHECK(region.is_valid());

		SUBCASE("'ProcessInfo' should not report dangling region") {
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_REGION_COUNT), 0);
		}

		SUBCASE("Setters/getters should work") {
			bool initial_use_edge_connections = navigation_server->region_get_use_edge_connections(region);
			navigation_server->region_set_enter_cost(region, 0.55);
			navigation_server->region_set_navigation_layers(region, 5);
			navigation_server->region_set_owner_id(region, ObjectID((int64_t)7));
			navigation_server->region_set_travel_cost(region, 0.66);
			navigation_server->region_set_use_edge_connections(region, !initial_use_edge_connections);
			navigation_server->process(0.0); // Give server some cycles to commit.

			CHECK_EQ(navigation_server->region_get_enter_cost(region), doctest::Approx(0.55));
			CHECK_EQ(navigation_server->region_get_navigation_layers(region), 5);
			CHECK_EQ(navigation_server->region_get_owner_id(region), ObjectID((int64_t)7));
			CHECK_EQ(navigation_server->region_get_travel_cost(region), doctest::Approx(0.66));
			CHECK_EQ(navigation_server->region_get_use_edge_connections(region), !initial_use_edge_connections);
		}

		SUBCASE("'ProcessInfo' should report region with active map") {
			RID map = navigation_server->map_create();
			CHECK(map.is_valid());
			navigation_server->map_set_active(map, true);
			navigation_server->region_set_map(region, map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_REGION_COUNT), 1);
			navigation_server->region_set_map(region, RID());
			navigation_server->free(map);
			navigation_server->process(0.0); // Give server some cycles to commit.
			CHECK_EQ(navigation_server->get_process_info(NavigationServer3D::INFO_REGION_COUNT), 0);
		}

		SUBCASE("Queries against empty region should return empty or invalid values") {
			CHECK_EQ(navigation_server->region_get_connections_count(region), 0);
			CHECK_EQ(navigation_server->region_get_connection_pathway_end(region, 55), Vector3());
			CHECK_EQ(navigation_server->region_get_connection_pathway_start(region, 55), Vector3());
		}

		navigation_server->free(region);
	}
}
} //namespace TestNavigationServer3D

#endif // TEST_NAVIGATION_SERVER_3D_H
