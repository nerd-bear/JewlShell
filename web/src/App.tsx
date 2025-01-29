import TerminalWindow from './components/TerminalWindow';
import Footer from './components/Footer';
import NavButtons from './components/NavButtons';
import WhatIsJSH from './components/WhatIsJSH';

function App() {
  return (
    <div className="flex flex-col text-center bg-[#161b22] h-screen w-screen">
      <header id="header" className="slide-down py-6">
        <a href="https://jewel.sh">
          <div className="container mx-auto px-4 text-center">
            <h1 className="text-5xl font-bold mb-4 transition-colors duration-300 text-white">Jewel Shell</h1>
          </div>
        </a>
      </header>

      <TerminalWindow />

      <main className="container mx-auto px-4 mt-10 bg-[#161b22]">
        <h2 className="font-bold text-3xl text-white bg-[#161b22]">Get Started</h2>
        <hr className="mt-4 mb-6 border-gray-800 bg-[#161b22]" />
        <WhatIsJSH />

        <NavButtons/>
      </main>

      <Footer/>
    </div>
  );
}

export default App;