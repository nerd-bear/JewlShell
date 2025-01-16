function NavButtons() {
    return (
        <div className="flex gap-4 justify-center mt-8 mb-5 bg-[#161b22]">
            <a href="https://jewl.sh/download" className="bg-[#161b22] border border-blue-500 p-4 rounded-lg hover:bg-blue-600/80 transition-colors text-white">Download Latest</a>
            <a href="https://jewl.sh/docs" className="bg-[#161b22] border p-4 rounded-lg hover:bg-gray-600/80 transition-colors text-white">Documentation</a>
        </div>
    );
}

export default NavButtons;