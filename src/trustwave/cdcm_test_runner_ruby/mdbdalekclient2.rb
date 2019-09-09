#!/usr/bin/env ruby

# Majordomo Protocol client example. Uses the mdcliapi2 API to hide all MDP aspects
#
# Author : Tom van Leeuwen <tom@vleeuwen.eu>
# Based on Python example by Min RK

require './mdbdalekcliapi2.rb'
require './testRunner.rb'
require './myLogger.rb'
require 'json'

runner = Test_Runner.new
runner.run_test_from_file("actions.xml")
END {
    puts "closing log file"
   log.close
}
