#!/usr/bin/env ruby

# Majordomo Protocol client example. Uses the mdcliapi2 API to hide all MDP aspects
#
# Author : Tom van Leeuwen <tom@vleeuwen.eu>
# Based on Python example by Min RK

require './mdcliapi2.rb'
require './testRunner.rb'
require './myLogger.rb'
require 'json'

xml_path = ARGV[0]
if  xml_path.nil? || xml_path.empty?
    xml_path="actions.xml"
    puts "please specify the path of tests xml. currently using default: " + xml_path
end

runner = Test_Runner.new
runner.run_test_from_file(xml_path)
END {
    puts "closing log file"
   log.close
}
