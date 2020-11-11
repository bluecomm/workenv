# include <iostream>
#include "bundler.pb.h"

int main() {

    BundlerToken bundler_token;                                                             
    std::string bundler_token_str;                                                                      
                                                                                                        
    // init bundler token                                                                               
    bundler_token.set_input_device_type(BIGPIPE);                                                       
    BigpipeState bp_state;                                                                              
    bp_state.set_pipe_name("dayu01-bigpipe");                                                           
    bp_state.set_pipelet_index(11);                                                                     
    bp_state.set_pipelet_serial_number(17);                                                             
    bp_state.set_cur_msg_id(4096);                                                                      
    bp_state.set_cur_seq_id(8192);                                                                      
    bp_state.set_pipe_token("pipe_token");                                                              
    bp_state.mutable_sub_progress()->add_tag("invalid_tag");                                            
    bp_state.mutable_sub_progress()->add_log_time("2018-08-13 11:10:34");                               
    bp_state.set_pipe_user_name("pipe_user_name");                                                      
    bp_state.set_pipe_password("pipe_password");                                                        
    bundler_token.mutable_bigpipe_state()->CopyFrom(bp_state);                                          
    bundler_token.mutable_file_slice_state()->set_file_list_name("filelist.txt");                       
    bundler_token.mutable_file_slice_state()->set_file_count(1);                                        
    bundler_token.set_output_path("/tm/nfs_data/tmp/");                                                 
    bundler_token.add_output_task_groups("Bundler4UpdaterGroup");                                       

    std::cout << "1 ---------------------------" << std::endl;
    std::cout << bundler_token.input_device_type() << std::endl;                                        
    std::cout << bundler_token.bigpipe_state().pipe_name() << std::endl;                                
    std::cout << bundler_token.file_slice_state().file_list_name() << std::endl;                        
    std::cout << bundler_token.output_path() << std::endl;                                              
    std::cout << bundler_token.output_task_groups(0) << std::endl;                                      
    std::cout << std::endl << bundler_token.DebugString() << std::endl; 

    bundler_token.SerializeToString(&bundler_token_str);
    BundlerToken bundler_token_tmpa;                                                     
    bundler_token_tmpa.ParseFromString(bundler_token_str);

    std::cout << "2 ---------------------------" << std::endl;
    std::cout << bundler_token_tmpa.input_device_type() << std::endl;                                
    std::cout << bundler_token_tmpa.bigpipe_state().pipe_name() << std::endl;                        
    std::cout << bundler_token_tmpa.file_slice_state().file_list_name() << std::endl;                
    std::cout << bundler_token_tmpa.output_path() << std::endl;                                      
    std::cout << bundler_token_tmpa.output_task_groups(0) << std::endl;                              
    std::cout << std::endl << bundler_token_tmpa.DebugString() << std::endl;  
}
