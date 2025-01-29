import { useState, useEffect } from 'react';

function TerminalWindow() {
    const [cursorVisible, setCursorVisible] = useState(true);

    useEffect(() => {
        const interval = setInterval(() => {
            setCursorVisible((prev) => !prev);
        }, 500);

        return () => clearInterval(interval); // Cleanup interval on component unmount
    }, []);

    return (
        <div className="container rounded-lg w-[80%] bg-[#2d2d2d] mx-auto shadow-lg border border-[#444] mt-8 text-left">
            <div className="flex justify-between items-center p-2 bg-[#1e1e1e] rounded-t-lg">
                <h3 className="ml-2 text-sm text-white">Jewel Shell</h3>
                <div className="flex gap-2 mr-2">
                    <span className="w-3 h-3 bg-[#00ca4e] rounded-full flex items-center justify-center text-[#00ca4e] text-xs hover:bg-[#00ca4e]/80 transition-colors cursor-pointer">□</span>
                    <span className="w-3 h-3 bg-[#ffbd44] rounded-full flex items-center justify-center text-[#ffbd44] text-xs hover:bg-[#ffbd44]/80 transition-colors cursor-pointer">-</span>
                    <span className="w-3 h-3 bg-[#ff605c] rounded-full flex items-center justify-center text-[#ff605c] text-xs hover:bg-[#ff605c]/80 transition-colors cursor-pointer">×</span>
                </div>
            </div>

            <div className="p-6 text-gray-400">
                <p>Welcome to JewelShell!<br/><br/>Use help/h/info to get detailed information about commands and terminal structure.<br/><br/> C:/WINDOWS/system32 :
                    <span id="cursor" className="relative font-extrabold text-xl">
                        {cursorVisible ? "_" : " "}
                    </span>
                </p>
            </div>
        </div>
    );
}

export default TerminalWindow;