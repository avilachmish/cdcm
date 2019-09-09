#!/usr/bin/env ruby

require './myLogger.rb'
require './mdbdalekcliapi2'
require './results_verification.rb'

###########################################################################
# Class: Test_Runner
#
#
###########################################################################
class Test_Runner

    def initialize
        @client = MajorDomoClient.new('tcp://localhost:5555')
        @sessions = Array.new
        @assets_list = Hash.new { | asset_name , asset_details |}
        @actions_list = Hash.new { | action_name , action_params |}
        @verification_methods_list  = Hash.new { | vm_name , vm_params |}
        @verifier = Verifier.new
    end

    ########################################
    #
    ########################################
    def sumerize_results
        total_num_of_session_items = 0
        num_passed = 0
        num_failed = 0
        num_other = 0
        @client.sessions.each {
            |session|
            session.session_items.each {
                |session_item|
                total_num_of_session_items += 1
                case session_item.verification_ctx.vm_result
                when "passed"
                    num_passed += 1
                when "failed"
                    num_failed += 1
                else
                    num_other += 1
                end
            }
        }
        log.info ("#{self.class.name}::#{__callee__}") {"run " + total_num_of_session_items.to_s + " session items. passed [" + num_passed.to_s + "] failed [" + num_failed.to_s + "] other [" + num_other.to_s + "]"}
        puts "===================================\nrun " + total_num_of_session_items.to_s + " session items. \npassed [" + num_passed.to_s + "] \nfailed [" + num_failed.to_s + "] \nother [" + num_other.to_s + "]\n==================================="

    end
    ########################################
    #
    ########################################
    def run_test_from_file(path)
        log.info ("#{self.class.name}::#{__callee__}") {"\n\n\nstart of run_test_from_file"}
        load_sessions_from_xml_file(path)
        @client.load_sessions(@sessions)
        #run all sessions
        @client.execute_sessions_sequentially
        sumerize_results
        log.info ("#{self.class.name}::#{__callee__}") {"=========================Done runing all sesssions. results dump: ========================="}
        log.info ("#{self.class.name}::#{__callee__}") {@client.dump}
        log.info ("#{self.class.name}::#{__callee__}") {"end of run_test_from_file"}
    end

    ########################################
    # map between action name and action params (array of strings)
    ########################################
    def read_actions root_element
        log.info ("#{self.class.name}::#{__callee__}") {"reading actions from xml"}
        root_element.elements.each("actions/action") {
            |action|
            name = action.attributes["name"]
            params_str = action.attributes["params"]
            params_vec = params_str.delete(" ").split(",")
            @actions_list[name] = params_vec
        }
    end

    ########################################
    # map between vm_name and vm_params (array of strings)
    ########################################
    def read_verification_methods root_element
        log.info ("#{self.class.name}::#{__callee__}") {"reading verification methods from xml"}
        root_element.elements.each("verification_methods/verification_method") {
            |verification_method|
            name = verification_method.attributes["vm_name"]
            params_str = verification_method.attributes["vm_params"]
            params_vec = params_str.delete(" ").split(",")
            @verification_methods_list[name] = params_vec
        }
    end

    ########################################
    # map between asset name and asset_details
    ########################################
    def read_assets root_element
        log.info ("#{self.class.name}::#{__callee__}") {"reading assets from xml"}
        root_element.elements.each("assets/asset") {
            |asset|
            name = asset.attributes["name"]
            addr = asset.attributes["address"]
            user = asset.attributes["user"]
            pass = asset.attributes["pass"]
            domain = asset.attributes["domain"]
            workstation = asset.attributes["workstation"]
            @assets_list[name] = Asset_Details.new(addr, user, pass, domain,   workstation)
        }
    end

    ########################################
    #
    #
    ########################################
    def load_sessions_from_xml_file(path)
        xmlfile = File.new(path)
        xmldoc = Document.new(xmlfile)
        log.info ("#{self.class.name}::#{__callee__}") {"xml opened: " + path}

        root_element = xmldoc.root()
        # read actions snippet
        read_actions(root_element)
        read_verification_methods(root_element)
        read_assets(root_element)

        #for each 'session'
        root_element.elements.each("sessions/session") {
            # extract the asset details
            |xml_session|
            #create Session object
            tmp_session =  Session.new
            asset_name = xml_session.attributes["asset_name"]
            #tmp_session.set_asset(@assets_list[asset_name])
            tmp_session.asset_details = @assets_list[asset_name]

            session_name = xml_session.attributes["session_name"]
            tmp_session.session_name = session_name

            session_description = xml_session.attributes["description"]
            tmp_session.session_description = session_description

            #extract the session_items and add to session
            xml_session.elements.each("session_item") {
                |xml_session_item|
                action_name = xml_session_item.attributes["action"]
                # get the list of params required for the specific action
                action_params_names_vec = @actions_list[action_name]
                # build a vector that hold pair of [param , param_value]
                action_params = Array.new
                action_params_names_vec.each {
                    |param|
                    param_val = xml_session_item.attributes[param]
                    param_pair = [param , param_val]
                    action_params.push(param_pair)
                }

                ver_method = xml_session_item.attributes["verification_method"]
                # get the list of params required for the specific action
                vm_params_names_vec = @verification_methods_list[ver_method]
                # build a vector that hold pair of [param , param_value]
                verification_params = Array.new
                vm_params_names_vec.each {
                    |param|
                    param_val = xml_session_item.attributes[param]
                    param_pair = [param , param_val]
                    verification_params.push(param_pair)
                }
                tmp_verification_ctx = Verification_Ctx.new(ver_method, verification_params)
                tmp_session.add_session_item(action_name, action_params, tmp_verification_ctx)
            }

            @sessions.push(tmp_session)
        }
    end


=begin
    ########################################
    #
    ########################################
    def run_test_tryout
        log.info ("#{self.class.name}::#{__callee__}") {"\n\n\nstart of run_test_tryout"}

        log.info "#{__FILE__} :: #{__LINE__}"
        log.info ('initialize') { "Initializing..." }
        log.info ("#{__method__}") { "Initializing..." }

        asset1 = Asset_Details.new("192.168.120.240",  "rfrenkel", "lech_tkawed", "Trustwave", "WORKSTATION1")
        session1 = Session.new
        session1.asset_detals = asset1
        session_item_id = session1.add_session_item("enumerate", "SOFTWARE\\\\Microsoft\\\\Windows NT\\\\CurrentVersion")
        log.info ("#{self.class.name}::#{__callee__}") {session1.dump}

        @client.add_session(session1)
        @client.execute_sessions_sequentially
        log.info ("#{self.class.name}::#{__callee__}") {@client.dump}

        log.info ("#{self.class.name}::#{__callee__}") {"end of run_test_tryout"}
    end
=end

    ########################################
    # push session to the end of sessions list
    ########################################
    def add_session(session)
        @sessions.push(session)
    end
end #end of class Test_Runner
