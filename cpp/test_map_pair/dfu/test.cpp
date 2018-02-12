
TEST(TestUtils, test_cal_bc_directional_info) {
    std::string bc_directional_info;
    uint32_t age_id;
    uint32_t gender_id,
    std::string interests_id_str;

    bc_directional_info = "5|2|pid,cid|912,1012,10,9|mob_nettype|mob_os_id|"
            "forum_dir|forum_second_dir|wday|hour";
    ASSERT_TRUE(cal_bc_directional_info(bc_directional_info, age_id, gender_id, interests_id_str));
    ASSERT_EQ(5, age_id);
    ASSERT_EQ(2, gender_id);
    ASSERT_EQ(interests_id_str.compare("912,1012,10,9"));

    bc_directional_info = "8|3|pid,cid|912|mob_nettype|mob_os_id|"
            "forum_dir|forum_second_dir|wday|hour";
    ASSERT_TRUE(cal_bc_directional_info(bc_directional_info, age_id, gender_id, interests_id_str));
    ASSERT_EQ(8, age_id);
    ASSERT_EQ(3, gender_id);
    ASSERT_EQ(interests_id_str.compare("912"));

    bc_directional_info = "a|b|pid,cid|912,1012,10,9|mob_nettype|mob_os_id|"
            "forum_dir|forum_second_dir|wday|hour";
    ASSERT_TRUE(cal_bc_directional_info(bc_directional_info, age_id, gender_id, interests_id_str));
    ASSERT_EQ(0, age_id);
    ASSERT_EQ(0, gender_id);
    ASSERT_EQ(interests_id_str.compare("912"));

    bc_directional_info = "5|2|pid,cid|912,1012,10,9";
    ASSERT_FALSE(cal_bc_directional_info(bc_directional_info, age_id, gender_id, interests_id_str));
}

TEST(TestUtils, test_calc_age_gender_id) {
    uint32_t age_id;
    uint32_t gender_id;
    ASSERT_TRUE(calc_age_gender_id(0, 1, age_id, gender_id));
    ASSERT_EQ(0, age_id);
    ASSERT_EQ(1, gender_id);
    ASSERT_TRUE(calc_age_gender_id(4, 2, age_id, gender_id));
    ASSERT_EQ(4, age_id);
    ASSERT_EQ(2, gender_id);
    ASSERT_TRUE(calc_age_gender_id(5, 2, age_id, gender_id));
    ASSERT_EQ(5, age_id);
    ASSERT_EQ(2, gender_id);
    ASSERT_TRUE(calc_age_gender_id(7, 1, age_id, gender_id));
    ASSERT_EQ(7, age_id);
    ASSERT_EQ(1, gender_id);
    ASSERT_FALSE(calc_age_gender_id(8, 1, age_id, gender_id));
    ASSERT_FALSE(calc_age_gender_id(5, 3, age_id, gender_id));
    ASSERT_FALSE(calc_age_gender_id(8, 3, age_id, gender_id));
}

TEST(TestUtils, test_calc_interests_id) {
    uint32_t interest_id = 0;
    uint32_t interest_second_id = 0;
    ASSERT_TRUE(calc_interests_id(1, interest_id, interest_second_id));
    ASSERT_EQ(1, interest_id);
    ASSERT_EQ(1, interest_second_id);
    ASSERT_TRUE(calc_interests_id(1508, interest_id, interest_second_id));
    ASSERT_EQ(15, interest_id);
    ASSERT_EQ(1508, interest_second_id);
    ASSERT_TRUE(calc_interests_id(26, interest_id, interest_second_id));
    ASSERT_EQ(26, interest_id);
    ASSERT_EQ(26, interest_second_id);
    ASSERT_TRUE(calc_interests_id(90204, interest_id, interest_second_id));
    ASSERT_EQ(9, interest_id);
    ASSERT_EQ(90204, interest_second_id);
    ASSERT_TRUE(calc_interests_id(409060, interest_id, interest_second_id));
    ASSERT_EQ(40, interest_id);
    ASSERT_EQ(409060, interest_second_id);
}

TEST(TestUtils, test_calc_forum_dir_id) {
    std::string forum_dir_str;
    std::string forum_second_dir_str;
    uint32_t forum_dir_id;
    uint32_t forum_second_dir_id;
    forum_dir_str = "Hello";
    forum_second_dir_str = "World";
    ASSERT_TRUE(forum_dir_str, forum_second_dir_str, forum_dir_id, forum_second_dir_id);
    ASSERT_EQ(0, forum_dir_id);
    ASSERT_EQ(0, forum_second_dir_id);
}

