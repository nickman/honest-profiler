/**
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
 */
package com.heliosapm.profiler;

import java.io.File;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.insightfullogic.honest_profiler.core.Conductor;
import com.insightfullogic.honest_profiler.core.parser.LogEventListener;
import com.insightfullogic.honest_profiler.core.parser.LogParser;
import com.insightfullogic.honest_profiler.core.parser.LogParser.AmountRead;
import com.insightfullogic.honest_profiler.core.parser.Method;
import com.insightfullogic.honest_profiler.core.parser.StackFrame;
import com.insightfullogic.honest_profiler.core.parser.TraceStart;
import com.insightfullogic.honest_profiler.ports.sources.FileLogSource;

/**
 * <p>Title: StreamFromLog</p>
 * <p>Description: </p> 
 * <p>Company: Helios Development Group LLC</p>
 * @author Whitehead (nwhitehead AT heliosdev DOT org)
 * <p><code>com.heliosapm.profiler.StreamFromLog</code></p>
 */

public class StreamFromLog implements LogEventListener {
	final File f;
	final FileLogSource fSource;
	final LogParser parser;
	final Logger log = LoggerFactory.getLogger(getClass());
	final Conductor conductor;
	int entries = 0;
	int t = 0;
	int x = 0;
	boolean eol = false;
	boolean rolled = false;
	Thread ageThread = null;
	
	public static final int MOD = 100;
	public static final long MAX_AGE = 30000;
	
	/**
	 * Creates a new StreamFromLog
	 * @param logFileName The file to read
	 */
	public StreamFromLog(final String logFileName) {
		f = new File(logFileName);
		fSource = new FileLogSource(f);
		parser = new LogParser(log, this);
		conductor = new Conductor(log, fSource, parser, true);
	}
	
	public void go() {
		while(true) {
			if(eol) break;
			if(conductor.poll()) {
				t++;
			} else {
				x++;
				log("Poll false");
			}
		}
//		for(int i = 0; i < 100; i++) {
//			conductor.poll();
//		}
	}
	
	public void rollToEnd() {
		int ff = 0;
		while(parser.readRecord(fSource.read())!=AmountRead.NOTHING) { ff++; }
		rolled = true;
		log("Rolled to end: %s", ff);
		ageThread = new Thread("AgeThread") {
			public void run() {
				while(true) {
					final long now = System.currentTimeMillis();
					final long age = f.lastModified();
					if(now-age > MAX_AGE) {
						eol = true;
						log("File aged out");
						break;
					}
					try { Thread.currentThread().join(100); } catch (Exception x) {/* No Op */}
				}
			}
		};
		ageThread.setDaemon(true);
		ageThread.start();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		log("Honest Log Reader");
		StreamFromLog sfl = new StreamFromLog("/tmp/log.hpl");
		sfl.rollToEnd();
		sfl.go();

	}
	
	public static void log(final Object fmt, final Object...args) {
		System.out.println(String.format(fmt.toString(), args));
	}

	@Override
	public void handle(final TraceStart traceStart) {
		if(!rolled) return;
		log("TraceStart: %s", traceStart);
		entries++;
		if(entries%MOD==0) log("Lines read: %s", entries);
	}

	@Override
	public void handle(final StackFrame stackFrame) {
		if(!rolled) return;
		log("StackFrame: %s", stackFrame);
		entries++;
		if(entries%MOD==0) log("Lines read: %s", entries);
	}

	@Override
	public void handle(final Method newMethod) {
		if(!rolled) return;
		log("Method: %s", newMethod);
		entries++;
		if(entries%MOD==0) log("Lines read: %s", entries);
	}

	@Override
	public void endOfLog() {
		if(!rolled) return;
		eol = true;
		log("\n\t===========\n\tEND OF LOG\n\tEntries: %s\n\t===========", entries);
		
	}

}
