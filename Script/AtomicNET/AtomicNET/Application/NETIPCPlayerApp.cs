using System;
using System.Reflection;

namespace AtomicEngine
{
    public partial class NETIPCPlayerApp : IPCPlayerApp
    {
        /// <summary>
        ///  IPC Player App used with the Atomic Editor
        /// </summary>
        /// <param name="args"></param>
        /// <param name="headless"></param>
        /// <returns></returns>
        public static NETIPCPlayerApp Create(string[] args, bool headless = false)
        {
            // Initialize AtomicNET
            AtomicNET.Initialize();

            var app = CreateInternal();

            app.Initialize();

            RegisterSubsystems();
            
            ExecuteAtomicMain(args);

            return app;
        }


    }

}
