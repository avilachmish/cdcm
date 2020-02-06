#!/usr/bin/env ruby

# Majordomo Protocol client example. Uses the mdcliapi2 API to hide all MDP aspects
#
# Author : Tom van Leeuwen <tom@vleeuwen.eu>
# Based on Python example by Min RK
folder = File.expand_path('.',__dir__)
$:.unshift(folder) unless $:.include?(folder)
require 'mdcliapi2.rb'
require 'testRunner.rb'
require_relative 'myLogger.rb'
require 'json'

#this if prevents this file to be executing when using require 'mdclient2.rb' (currently from run_multiple_clients.rb)
if __FILE__ == $0
    xml_path = ARGV[0]
    if  xml_path.nil? || xml_path.empty?
        xml_path="actions.xml"
        puts "please specify the path of tests xml. currently using default: " + xml_path
    end

    #client id is used by the logger to create different file name for each client
    $client_id = ARGV[1]
    if  $client_id.nil? || $client_id.empty?
        $client_id="1"
        puts "please specify the client id (1,2,...) as argv. currently using default: " + $client_id
    end

    runner = Test_Runner.new
    runner.run_test_from_file(xml_path)
    END {
        puts "closing log file"
        log.close
    }
end
